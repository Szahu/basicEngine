#pragma once

#include "Engine/Renderer/FrameBuffer.h"
#include "Engine/Events/ApplicationEvent.h"

namespace Engine
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer();
		virtual ~OpenGLFrameBuffer() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;
			 
		virtual const uint32_t GetTextureID() const override { return m_TextureColorBuffer; }
		virtual void BindTexture() const override;
		virtual void UnbindTexture() const override;

		virtual void OnEvent(Event& e) override;

		bool OnWindowResize(WindowResizeEvent& e);

	private:
		uint32_t m_FrameBuffer;
		uint32_t m_RenderBuffer;
		uint32_t m_TextureColorBuffer;
	};
}