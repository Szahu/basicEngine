#pragma once

#include "Engine/Renderer/Texture.h"
#include <glad/glad.h>

namespace Engine
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);
		OpenGLTexture2D(uint32_t width, uint32_t height);
		~OpenGLTexture2D();

		virtual uint32_t GetWidth() override { return m_Width; }
		virtual uint32_t GetHeight() override { return m_Height; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual const uint32_t GetID() const override { return m_RendererID; }
		
		virtual const void SetType(const std::string& type) override { m_Type = type; }
		virtual const std::string& GetType() const override { return m_Type; }

		virtual const std::string& GetPath() const override { return m_Path; }

		virtual void Bind(uint32_t slot = 0) const override;
		virtual void Unbind() const override;

	private:
		std::string m_Path;
		std::string m_Type;
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_RendererID;
		GLenum m_InternalFormat;
		GLenum m_DataFormat;
	};
}