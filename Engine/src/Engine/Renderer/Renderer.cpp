#include "EGpch.h"
#include "Renderer.h"

#include "Engine/Platform/OpenGl/OpenGLShader.h"


namespace Engine
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(PerspectiveCamera& camera, PointLight& light)
	{
		m_SceneData->m_Camera = &camera;
		m_SceneData->light = light;
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Engine::Ref<VertexArray>& vertexArray, const Engine::Ref<Shader>& shader, const glm::mat4& transform, uint32_t instances)
	{
		shader->Bind();
		shader->SetMat4("u_ViewProjectionMatrix", m_SceneData->m_Camera->GetViewProjectionMatrix());
		shader->SetMat4("u_Transform", transform);
		shader->SetFloat3("u_CameraPosition", m_SceneData->m_Camera->GetPosition());

		shader->SetFloat3("u_Material.ambient", glm::vec3(1.0f));
		shader->SetFloat3("u_Material.diffuse", glm::vec3(1.0f));
		shader->SetFloat3("u_Material.specular", glm::vec3(1.0f));
		shader->SetFloat1("u_Material.shininess", 1.0f);

		shader->SetFloat3("u_PointLight.position", m_SceneData->light.GetPosition());
		shader->SetFloat3("u_PointLight.ambient", m_SceneData->light.GetAmbient());
		shader->SetFloat3("u_PointLight.diffuse", m_SceneData->light.GetDiffuse());
		shader->SetFloat3("u_PointLight.specular", m_SceneData->light.GetSpecular());

		if (instances == 0)
		{
			vertexArray->Bind();
			RenderCommand::DrawIndexed(vertexArray);
		}
		else
		{
			vertexArray->Bind();
			RenderCommand::DrawInstanced(vertexArray, instances);
		}
	}

	void Renderer::Submit(Model& model, const Engine::Ref<Shader>& shader, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4("u_ViewProjectionMatrix", m_SceneData->m_Camera->GetViewProjectionMatrix());
		shader->SetMat4("u_Transform", transform);
		shader->SetFloat3("u_CameraPosition", m_SceneData->m_Camera->GetPosition());

		shader->SetFloat3("u_Material.ambient", glm::vec3(1.0f));
		shader->SetFloat3("u_Material.diffuse", glm::vec3(1.0f));
		shader->SetFloat3("u_Material.specular", glm::vec3(1.0f));
		shader->SetFloat1("u_Material.shininess", 1.0f);

		shader->SetFloat3("u_PointLight.position", m_SceneData->light.GetPosition());
		shader->SetFloat3("u_PointLight.diffuse", m_SceneData->light.GetDiffuse());
		shader->SetFloat3("u_PointLight.ambient", m_SceneData->light.GetAmbient());
		shader->SetFloat3("u_PointLight.specular", m_SceneData->light.GetSpecular());

		model.Draw(shader);
	}


}
