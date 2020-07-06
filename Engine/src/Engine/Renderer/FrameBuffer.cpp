#include "EGpch.h"
#include "FrameBuffer.h"

#include "Renderer.h"
#include "Engine/Platform/OpenGl/OpenGLFrameBuffer.h"

namespace Engine
{
	Ref<FrameBuffer> Engine::FrameBuffer::Create(const glm::vec2& size , ATTACHMENT_TYPE type)
	{
		
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: EG_CORE_ASSERT(false, "RendererAPI None is not supported!");  return nullptr;
			case RendererAPI::API::OpenGL:
			{
				if(type == ATTACHMENT_TYPE::NONE) return  std::make_shared<DefaultOpenGlFrameBuffer>();
				else return  std::make_shared<OpenGLFrameBuffer>(size, type); 
			}
		}

		EG_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;

	}
}


