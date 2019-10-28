#include "EGpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Engine/Platform/OpenGl/OpenGLVertexArray.h"

namespace Engine
{
	Ref<VertexArray> Engine::VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: EG_CORE_ASSERT(false, "RendererAPI None is not supported!");  return nullptr;
			case RendererAPI::API::OpenGL:  return  std::make_shared<OpenGLVertexArray>();
		}

		EG_CORE_ASSERT(false, "Unknow RendererAPI!");
		return nullptr;

	}
}

