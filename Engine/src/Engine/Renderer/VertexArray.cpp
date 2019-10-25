#include "EGpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Engine/Platform/OpenGl/OpenGLVertexArray.h"

namespace Engine
{
	VertexArray* Engine::VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: EG_CORE_ASSERT(false, "RendereAPI None is not supprtoed!");  return nullptr;
			case RendererAPI::API::OpenGL:  return new OpenGLVertexArray();
		}

		EG_CORE_ASSERT(false, "Unknow RendererAPI!");
		return nullptr;

	}
}

