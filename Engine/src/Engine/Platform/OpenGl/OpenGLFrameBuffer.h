#pragma once

#include "Engine/Renderer/FrameBuffer.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Core/Application.h"

namespace Engine
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const glm::vec2& size);
		virtual ~OpenGLFrameBuffer() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;
			 
		//virtual const uint32_t GetTextureID() const override { return m_TextureColorBuffer; }
		virtual const uint32_t GetTextureID() const override { return m_ScreenTexture; }
		
		virtual void BindTexture() const override;
		virtual void UnbindTexture() const override;

		virtual void SetTextureSize(const glm::vec2& vec) override;
		virtual const glm::vec2& GetTextureSize() const override { return m_TextureSize; }

		virtual void OnEvent(Event& e) override;

		bool OnWindowResize(WindowResizeEvent& e);

	private:

		void UpdateSize();

	private:
		uint32_t m_FrameBuffer, m_SampledFrameBuffer, m_DistFrameBuffer;
		uint32_t m_RenderBuffer;
		uint32_t m_TextureColorBuffer, m_ScreenTexture;
		glm::vec2 m_TextureSize = { Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight() };
	};
}