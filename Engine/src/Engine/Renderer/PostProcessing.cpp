#include "EGpch.h"
#include "PostProcessing.h"

#include "Engine/Renderer/Renderer.h"

namespace Engine
{
	PostProcessingEffect::PostProcessingEffect(const char* libKey)
	{
		m_ShaderKey = libKey;
		m_FBO = FrameBuffer::Create({1280.0f, 720.0f}, FrameBuffer::ATTACHMENT_TYPE::COLOR);
	}

	void PostProcessingEffect::Procces()
	{
		m_FBO->SetTextureSize(Renderer::GetLastUsedFrameBuffer()->GetTextureSize());

		m_FBO->Bind();
		Engine::RenderCommand::Clear();
		Engine::RenderCommand::SetClearColor({ 135.0f / 255.0f, 206.0f / 255.0f, 250.0f / 255.0f, 1.0f });

		Renderer::GetShaderLibrary()->Get(m_ShaderKey)->Bind();
		Renderer::GetShaderLibrary()->Get(m_ShaderKey)->SetInt1("tex", 0);
		Renderer::GetLastUsedFrameBuffer()->BindTexture(0);
		Renderer::GetScreenQuad()->Bind();
		RenderCommand::DrawIndexed(Renderer::GetScreenQuad());

		m_FBO->Unbind();
		Engine::RenderCommand::Clear();

		Renderer::SetLastUsedFrameBuffer(m_FBO);
	}

}