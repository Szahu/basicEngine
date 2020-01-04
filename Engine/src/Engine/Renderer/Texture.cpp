#include "EGpch.h"
#include "Texture.h"

#include "Renderer.h"

#include "Engine/Platform/OpenGl/OpenGlTexture.h"

namespace Engine
{
	Ref<Texture2D> Engine::Texture2D::Create(const std::string& path, bool flipOnLoad)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: EG_CORE_ASSERT(false, "RendereAPI None is not supprtoed!");  return nullptr;
			case RendererAPI::API::OpenGL:  return  CreateRef<OpenGLTexture2D>(path, flipOnLoad);
		}

		EG_CORE_ASSERT(false, "Unknow RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Engine::Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: EG_CORE_ASSERT(false, "RendereAPI None is not supprtoed!");  return nullptr; break;
			case RendererAPI::API::OpenGL:  return  CreateRef<OpenGLTexture2D>(width, height); break;
		}

		EG_CORE_ASSERT(false, "Unknow RendererAPI!");
		return nullptr;
	}
}

