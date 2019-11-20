#include "EGpch.h"
#include "Renderer.h"

#include "Engine/Platform/OpenGl/OpenGLShader.h"

#include "glad/glad.h" //Temporary
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

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

		m_SceneData->m_MatricesUniformBuffer = UniformBuffer::Create(sizeof(glm::mat4) + sizeof(glm::vec3), 0);
		m_SceneData->m_LightsUniformBuffer = UniformBuffer::Create(sizeof(glm::vec4) + sizeof(PointLightData) * MAX_NUMBER_OF_POINTLIGHTS + sizeof(SpotLight) * MAX_NUMBER_OF_SPOTLIGHTS, 1);

		m_SceneData->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t WhiteTextureData = 0xffffffff;
		m_SceneData->WhiteTexture->SetData(&WhiteTextureData, sizeof(WhiteTextureData));
	}

	void Renderer::BeginScene()
	{
		Application::Get().GetViewportWindowPointer()->GetFrameBuffer()->Bind();

		Engine::RenderCommand::Clear();
		Engine::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 0.0f });
		glStencilMask(0xFF);

		m_SceneData->m_MatricesUniformBuffer->Bind();
		m_SceneData->m_MatricesUniformBuffer->AddSubData(0, sizeof(glm::mat4), &m_SceneData->m_Camera->GetViewProjectionMatrix()[0][0]);
		m_SceneData->m_MatricesUniformBuffer->AddSubData(sizeof(glm::mat4), sizeof(glm::vec3), &m_SceneData->m_Camera->GetPosition().x);

		m_SceneData->m_LightsUniformBuffer->Bind();

		std::vector<PointLight> pointlights = Scene::GetActiveScene().GetPointLights();
		std::vector<SpotLight> spotlights = Scene::GetActiveScene().GetSpotLights();

		glm::vec4 amountOfLights;
		amountOfLights.x = (float)Scene::GetActiveScene().GetPointLights().size();
		amountOfLights.y = (float)Scene::GetActiveScene().GetSpotLights().size();
		m_SceneData->m_LightsUniformBuffer->AddSubData(0, sizeof(glm::vec4), &amountOfLights.x);

		for (int i = 0; i < pointlights.size(); i++)
		{
			m_SceneData->m_LightsUniformBuffer->AddSubData(sizeof(glm::vec4) + i * sizeof(PointLightData), sizeof(PointLightData), &pointlights[i].GetLightData().Position.x);
		}

		for (int i = 0; i < spotlights.size(); i++)
		{
			m_SceneData->m_LightsUniformBuffer->AddSubData(sizeof(glm::vec4) + MAX_NUMBER_OF_POINTLIGHTS * sizeof(PointLightData) + i * sizeof(SpotLightData), sizeof(SpotLightData), &spotlights[i].GetLightData().Position.x);
		}

		m_SceneData->m_LightsUniformBuffer->Unbind();

		
	}

	void Renderer::EndScene()
	{
		
		Application::Get().GetViewportWindowPointer()->GetFrameBuffer()->Unbind();

		Engine::RenderCommand::Clear();

		glStencilMask(0x00);

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
		
		ProcessMaterial(material, shader);

		if (false)//(drawOutline)
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

		if (!drawOutline || drawOutline)
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
		
		ProcessMaterial(material, shader);

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

			//model.GetVertexArray()->Bind();
			//model.ProccesTextures(shader);
			//RenderCommand::DrawIndexed(model.GetVertexArray());
			//model.GetVertexArray()->Unbind();

			glEnable(GL_STENCIL_TEST);
		}
		
		glActiveTexture(GL_TEXTURE0);
	}

	void Renderer::ProcessMaterial(const Material& material, const Ref<Shader>& shader)
	{

		m_SceneData->WhiteTexture->Bind(0);
		m_SceneData->WhiteTexture->Bind(1);
		m_SceneData->WhiteTexture->Bind(2);

		shader->SetFloat3("u_Material.ambient", material.m_Ambient);
		shader->SetFloat3("u_Material.diffuse", material.m_Diffuse);
		shader->SetFloat3("u_Material.specular", material.m_Specular);
		shader->SetFloat1("u_Material.shininess", material.m_Shininess);

		if (material.m_DiffuseTexture != nullptr)
		{
			shader->SetInt1("texture_diffuse1", 0);
			material.m_DiffuseTexture->Bind();
		}

		if (material.m_NormalMap != nullptr)
		{
			shader->SetInt1("texture_normal1", 1);
			material.m_NormalMap->Bind(1);
		}

		if (material.m_HeightMap != nullptr)
		{
			shader->SetInt1("texture_height1", 2);
			material.m_HeightMap->Bind(2);
		}

		if (material.m_AmbientMap != nullptr)
		{
			shader->SetInt1("texture_specular1", 3);
			material.m_AmbientMap->Bind(3);
		}
	}


}
