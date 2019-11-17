#include "EGpch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Engine/Platform/OpenGl/OpenGLBuffer.h"

namespace Engine
{
	
	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{

		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: EG_CORE_ASSERT(false, "RendereAPI None is not supprtoed!");  return nullptr;
			case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		}

		EG_CORE_ASSERT(false, "Unknow RendererAPI!");
		return nullptr;

	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{

		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: EG_CORE_ASSERT(false, "RendereAPI None is not supprtoed!");  return nullptr;
			case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLIndexBuffer>(indices, count);
		}

		EG_CORE_ASSERT(false, "Unknow RendererAPI!");
		return nullptr;
		
	}

	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t baseIndex)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: EG_CORE_ASSERT(false, "RendereAPI None is not supprtoed!");  return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLUniformBuffer>(size, baseIndex);
		}

		EG_CORE_ASSERT(false, "Unknow RendererAPI!");
		return nullptr;
	}
}


