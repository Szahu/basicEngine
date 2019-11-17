#include "EGpch.h"
#include "OpenGLFrameBuffer.h"

#include "Application.h"

#include "glad/glad.h"

namespace Engine
{
	OpenGLFrameBuffer::OpenGLFrameBuffer(const glm::vec2& size)
	{
		glGenFramebuffers(1, &m_SampledFrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_SampledFrameBuffer);
		// create a multisampled color attachment texture
		glGenTextures(1, &m_TextureColorBuffer);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_TextureColorBuffer);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, size.x, size.y, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_TextureColorBuffer, 0);
		// create a (also multisampled) renderbuffer object for depth and stencil attachments
		glGenRenderbuffers(1, &m_RenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, size.x, size.y);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			EG_CORE_ERROR("ERROR::FRAMEBUFFER:: SampledFramebuffer is not complete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);



		glGenFramebuffers(1, &m_DistFrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_DistFrameBuffer);
		// create a color attachment texture
		glGenTextures(1, &m_ScreenTexture);
		glBindTexture(GL_TEXTURE_2D, m_ScreenTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ScreenTexture, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			EG_CORE_ERROR("ERROR::FRAMEBUFFER:: Dist framebuffer is not complete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);



		/*
		glGenFramebuffers(1, &m_FrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);

		glGenTextures(1, &m_TextureColorBuffer);
		glBindTexture(GL_TEXTURE_2D, m_TextureColorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureColorBuffer, 0);

		glGenRenderbuffers(1, &m_RenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			EG_CORE_ERROR("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		*/
	}

	Engine::OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{

	}

	void OpenGLFrameBuffer::Bind() const
	{

		glBindFramebuffer(GL_FRAMEBUFFER, m_SampledFrameBuffer);
		glEnable(GL_DEPTH_TEST);
		glViewport(0, 0, m_TextureSize.x, m_TextureSize.y);
		glStencilMask(0xFF);


		/*
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
		glEnable(GL_DEPTH_TEST);
		glViewport(0, 0, m_TextureSize.x, m_TextureSize.y);		

		glStencilMask(0xFF);
		*/
	}

	void OpenGLFrameBuffer::Unbind() const
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_SampledFrameBuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_DistFrameBuffer);
		glBlitFramebuffer(0, 0, m_TextureSize.x, m_TextureSize.y, 0, 0, m_TextureSize.x, m_TextureSize.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		glDisable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		UnbindTexture();

		/*
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		*/
	}

	void OpenGLFrameBuffer::BindTexture() const
	{
		glBindTexture(GL_TEXTURE_2D, m_ScreenTexture);
	}

	void OpenGLFrameBuffer::UnbindTexture() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OpenGLFrameBuffer::SetTextureSize(const glm::vec2& vec)
	{
		m_TextureSize = vec;
		UpdateSize();
	}

	void OpenGLFrameBuffer::OnEvent(Event& e) 
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(EG_BIND_EVENT_FN(OpenGLFrameBuffer::OnWindowResize));
	}

	bool OpenGLFrameBuffer::OnWindowResize(WindowResizeEvent& e)
	{
		//m_TextureSize = { e.GetWidth(), e.GetHeight() };

		//UpdateSize();

		return false;
	}

	void OpenGLFrameBuffer::UpdateSize()
	{

		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_TextureColorBuffer);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, m_TextureSize.x, m_TextureSize.y, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

		glBindTexture(GL_TEXTURE_2D, m_ScreenTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_TextureSize.x, m_TextureSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, m_TextureSize.x, m_TextureSize.y);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);


		/*
		BindTexture();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_TextureSize.x, m_TextureSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		UnbindTexture();

		glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_TextureSize.x, m_TextureSize.y);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		*/
	}
}


