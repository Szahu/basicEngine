#include "EGpch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Engine/Platform/OpenGl/OpenGLBuffer.h"

namespace Engine
{
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{

		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: EG_CORE_ASSERT(false, "RendereAPI None is not supprtoed!");  return nullptr;
			case RendererAPI::API::OpenGL:  return new OpenGLVertexBuffer(vertices, size);
		}

		EG_CORE_ASSERT(false, "Unknow RendererAPI!");
		return nullptr;

	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{

		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: EG_CORE_ASSERT(false, "RendereAPI None is not supprtoed!");  return nullptr;
			case RendererAPI::API::OpenGL:  return new OpenGLIndexBuffer(indices, count);
		}

		EG_CORE_ASSERT(false, "Unknow RendererAPI!");
		return nullptr;
		
	}
}


