#include "EGpch.h"
#include "FrameBuffer.h"

#include "Renderer.h"
#include "Engine/Platform/OpenGl/OpenGLFrameBuffer.h"

namespace Engine
{
	Ref<FrameBuffer> Engine::FrameBuffer::Create(const glm::vec2& size)
	{
		
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: EG_CORE_ASSERT(false, "RendererAPI None is not supported!");  return nullptr;
			case RendererAPI::API::OpenGL:  return  std::make_shared<OpenGLFrameBuffer>(size);
		}

		EG_CORE_ASSERT(false, "Unknow RendererAPI!");
		return nullptr;

	}
}


