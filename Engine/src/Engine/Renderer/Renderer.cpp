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

	void Renderer::BeginScene(OrtographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Engine::Ref<VertexArray>& vertexArray, const Engine::Ref<Shader>& shader, const glm::mat4 transform)
	{
		std::dynamic_pointer_cast<OpenGLShader>(shader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UplaodUniformMat4("u_ViewProjectionMatrix", m_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UplaodUniformMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}
