#include "EGpch.h"
#include "Renderer.h"

#include "Engine/Platform/OpenGl/OpenGLShader.h"

#include "glad/glad.h" //Temporary
#include "glm/gtc/matrix_transform.hpp"

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

	void Renderer::BeginScene(const Ref<FrameBuffer>& frameBuffer, const PerspectiveCamera& camera, const std::vector<PointLight*>& lights, ShaderLibrary* library)
	{
		m_SceneData->m_Camera = &camera;
		m_SceneData->m_Lights = lights;
		m_SceneData->m_ShaderLibrary = library;
		m_SceneData->m_FrameBuffer = frameBuffer;

		m_SceneData->m_FrameBuffer->Bind();

		Engine::RenderCommand::Clear();
		Engine::RenderCommand::SetClearColor({ 0.53f, 0.81f, 0.98f, 1.0f });

	}

	void Renderer::EndScene()
	{
		m_SceneData->m_FrameBuffer->Unbind();

		Engine::RenderCommand::Clear();

		//m_SceneData->m_FrameBuffer->BindTexture();
		//Engine::RenderCommand::RenderToScreen();
	}

	void Renderer::Submit(const Engine::Ref<VertexArray>& vertexArray, const MaterialStruct& material, const Transform& transform, bool drawOutline, const Engine::Ref<Shader>& shader,  uint32_t instances)
	{
		shader->Bind();
		shader->SetFloat3("u_FlatColor", glm::vec3(0.0f, 0.0f, 0.0f));
		shader->SetMat4("u_ViewProjectionMatrix", m_SceneData->m_Camera->GetViewProjectionMatrix());
		shader->SetMat4("u_Transform", transform);
		shader->SetFloat3("u_CameraPosition", m_SceneData->m_Camera->GetPosition());

		shader->SetFloat3("u_Material.ambient", material.m_Ambient);
		shader->SetFloat3("u_Material.diffuse", material.m_Diffuse);
		shader->SetFloat3("u_Material.specular", material.m_Specular);
		shader->SetFloat1("u_Material.shininess", material.m_Shininess);

		for (unsigned int i = 0; i < m_SceneData->m_Lights.size(); i++)
		{
			std::string location = "u_PointLights[" + std::to_string(i) + "].position";
			shader->SetFloat3(location, m_SceneData->m_Lights[i]->GetPosition());
			location = "u_PointLights[" + std::to_string(i) + "].ambient";
			shader->SetFloat3(location, m_SceneData->m_Lights[i]->GetAmbient());
			location = "u_PointLights[" + std::to_string(i) + "].diffuse";
			shader->SetFloat3(location, m_SceneData->m_Lights[i]->GetDiffuse());
			location = "u_PointLights[" + std::to_string(i) + "].specular";
			shader->SetFloat3(location, m_SceneData->m_Lights[i]->GetSpecular());
		}

		if (instances == 0)
		{
			if (drawOutline)
			{
				glStencilFunc(GL_ALWAYS, 1, 0xFF);
				glStencilMask(0xFF);

				vertexArray->Bind();
				RenderCommand::DrawIndexed(vertexArray);

				glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
				glStencilMask(0x00);

				glm::mat4 scaledTransform = transform;
				scaledTransform[0][0] += 0.15f;
				scaledTransform[1][1] += 0.15f;
				scaledTransform[2][2] += 0.15f;

				shader->SetMat4("u_Transform", scaledTransform);
				shader->SetFloat3("u_FlatColor", glm::vec3(1.0f, 165.0f / 255.0f, 0.0f));
				RenderCommand::DrawIndexed(vertexArray);
			}
			else
			{
				glStencilFunc(GL_ALWAYS, 1, 0xFF); // all fragments should update the stencil buffer

				vertexArray->Bind();
				RenderCommand::DrawIndexed(vertexArray);
			}
		}
		else
		{
			vertexArray->Bind();
			RenderCommand::DrawInstanced(vertexArray, instances);
		}
	}

	void Renderer::Submit(Model& model, const glm::mat4& transform)
	{
		Ref<Shader> shader = Renderer::m_SceneData->m_ShaderLibrary->Get("Material");
		shader->Bind();
		shader->SetMat4("u_ViewProjectionMatrix", m_SceneData->m_Camera->GetViewProjectionMatrix());
		shader->SetMat4("u_Transform", transform);
		shader->SetFloat3("u_CameraPosition", m_SceneData->m_Camera->GetPosition());

		shader->SetFloat3("u_Material.ambient", glm::vec3(1.0f));
		shader->SetFloat3("u_Material.diffuse", glm::vec3(1.0f));
		shader->SetFloat3("u_Material.specular", glm::vec3(1.0f));
		shader->SetFloat1("u_Material.shininess", 64.0f);

		for (unsigned int i = 0; i < m_SceneData->m_Lights.size(); i++)
		{
			std::string location = "u_PointLights[" + std::to_string(i) + "].position";
			shader->SetFloat3(location, m_SceneData->m_Lights[i]->GetPosition());
			location = "u_PointLights[" + std::to_string(i) + "].ambient";
			shader->SetFloat3(location, m_SceneData->m_Lights[i]->GetAmbient());
			location = "u_PointLights[" + std::to_string(i) + "].diffuse";
			shader->SetFloat3(location, m_SceneData->m_Lights[i]->GetDiffuse());
			location = "u_PointLights[" + std::to_string(i) + "].specular";
			shader->SetFloat3(location, m_SceneData->m_Lights[i]->GetSpecular());
		}

		model.Draw(shader);

	}


}
