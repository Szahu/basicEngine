#include "EGpch.h"
#include "Texture.h"

#include "Renderer.h"

#include "Engine/Platform/OpenGl/OpenGlTexture.h"

namespace Engine
{
	Ref<Texture2D> Engine::Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: EG_CORE_ASSERT(false, "RendereAPI None is not supprtoed!");  return nullptr;
			case RendererAPI::API::OpenGL:  return  std::make_shared<OpenGLTexture2D>(path);
		}

		EG_CORE_ASSERT(false, "Unknow RendererAPI!");
		return nullptr;
	}
}

