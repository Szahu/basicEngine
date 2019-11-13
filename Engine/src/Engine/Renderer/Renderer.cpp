#include "EGpch.h"
#include "Renderer.h"

#include "Engine/Platform/OpenGl/OpenGLShader.h"

#include "glad/glad.h" //Temporary
#include "glm/gtc/matrix_transform.hpp"

#include "Engine/Renderer/Scene.h"

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

	void Renderer::InitScene()
	{
		m_SceneData->m_Camera = &Scene::GetActiveScene().GetCamera().GetCamera();
		m_SceneData->m_ShaderLibrary = &Scene::GetActiveScene().GetShaderLibrary();
	}

	void Renderer::BeginScene()
	{
		Application::Get().GetViewportWindowPointer()->GetFrameBuffer()->Bind();

		Engine::RenderCommand::Clear();
		Engine::RenderCommand::SetClearColor({ 0.53f, 0.81f, 0.98f, 1.0f });
		glStencilMask(0xFF);

		for (int i = 0; i < m_SceneData->m_ShadersInUse.size(); i++)
		{
			m_SceneData->m_ShaderLibrary->Get(m_SceneData->m_ShadersInUse[i])->Bind();
			m_SceneData->m_ShaderLibrary->Get(m_SceneData->m_ShadersInUse[i])->SetFloat3("u_FlatColor", glm::vec3(0.0f, 0.0f, 0.0f));
			m_SceneData->m_ShaderLibrary->Get(m_SceneData->m_ShadersInUse[i])->SetMat4("u_ViewProjectionMatrix", m_SceneData->m_Camera->GetViewProjectionMatrix());
			m_SceneData->m_ShaderLibrary->Get(m_SceneData->m_ShadersInUse[i])->SetFloat3("u_CameraPosition", m_SceneData->m_Camera->GetPosition());
		}

	}

	void Renderer::EndScene()
	{
		Application::Get().GetViewportWindowPointer()->GetFrameBuffer()->Unbind();

		Engine::RenderCommand::Clear();

		//m_SceneData->m_FrameBuffer->BindTexture();
		//Engine::RenderCommand::RenderToScreen();
	}

	void Renderer::Submit(const Engine::Ref<VertexArray>& vertexArray, const Material& material, const Transform& transform, bool drawOutline, const std::string& libKey,  uint32_t instances)
	{
		//If did NOT find a shader
		if (std::find(m_SceneData->m_ShadersInUse.begin(), m_SceneData->m_ShadersInUse.end(), libKey) == m_SceneData->m_ShadersInUse.end())
		{
			m_SceneData->m_ShadersInUse.push_back(libKey);
		}

		Ref<Shader> shader = m_SceneData->m_ShaderLibrary->Get(libKey);

		shader->Bind();
		shader->SetMat4("u_Transform", transform);
		
		ProcessLightsAndMaterial(material, shader);

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

			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0x00);

			return;
		}

		if (!drawOutline)
		{
			glDisable(GL_STENCIL_TEST);

			vertexArray->Bind();
			RenderCommand::DrawIndexed(vertexArray);
			vertexArray->Unbind();
			glEnable(GL_STENCIL_TEST);
			return;
		}
		
		
		if (instances != 0)
		{
			vertexArray->Bind();
			RenderCommand::DrawInstanced(vertexArray, instances);
			vertexArray->Unbind();
		}
	}

	void Renderer::Submit(Model& model, const Material& material, const Transform& transform, bool drawOutline, const std::string& libKey)
	{

		if (std::find(m_SceneData->m_ShadersInUse.begin(), m_SceneData->m_ShadersInUse.end(), libKey) == m_SceneData->m_ShadersInUse.end())
		{
			m_SceneData->m_ShadersInUse.push_back(libKey);
		}

		Ref<Shader> shader = m_SceneData->m_ShaderLibrary->Get(libKey);
		shader->Bind();
		shader->SetMat4("u_Transform", transform);
		
		ProcessLightsAndMaterial(material, shader);

		if (!drawOutline || drawOutline)
		{
			glDisable(GL_STENCIL_TEST);

			for (int i = 0; i < model.GetMeshes().size(); i++)
			{
				model.GetMeshes()[i].ProccessMaterial(m_SceneData->m_ShaderLibrary->Get(libKey));
				model.GetMeshes()[i].GetVertexArray()->Bind();
				RenderCommand::DrawIndexed(model.GetMeshes()[i].GetVertexArray());
				model.GetMeshes()[i].GetVertexArray()->Unbind();
			}
			
			glEnable(GL_STENCIL_TEST);
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);

	}

	void Renderer::ProcessLightsAndMaterial(const Material& material, const Ref<Shader>& shader)
	{
		std::vector<PointLight> lights = Scene::GetActiveScene().GetLights();
		for (unsigned int i = 0; i < Scene::GetActiveScene().GetLights().size(); i++)
		{
			std::string location = "u_PointLights[" + std::to_string(i) + "].position";
			shader->SetFloat3(location, lights[i].GetPosition());
			location = "u_PointLights[" + std::to_string(i) + "].ambient";
			shader->SetFloat3(location, lights[i].GetAmbient());
			location = "u_PointLights[" + std::to_string(i) + "].diffuse";
			shader->SetFloat3(location, lights[i].GetDiffuse());
			location = "u_PointLights[" + std::to_string(i) + "].specular";
			shader->SetFloat3(location, lights[i].GetSpecular());
		}

		shader->SetFloat3("u_Material.ambient", material.m_Ambient);
		shader->SetFloat3("u_Material.diffuse", material.m_Diffuse);
		shader->SetFloat3("u_Material.specular", material.m_Specular);
		shader->SetFloat1("u_Material.shininess", material.m_Shininess);

		if (material.m_DiffuseTexture != nullptr)
		{
			shader->SetInt1("texture_diffuse1", 0);
			material.m_DiffuseTexture->Bind();
		}
	}


}
