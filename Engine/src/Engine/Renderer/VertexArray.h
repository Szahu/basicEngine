#pragma once

#include <memory>

#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/Shader.h"

namespace Engine
{
	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const Engine::Ref<VertexBuffer>& vertexBuffer, bool instancing = false, uint32_t divisor = 1) = 0;
		virtual void SetIndexBuffer(const Engine::Ref<IndexBuffer>& indexBuffer, int drawingMode = 0) = 0; //Change drawingMode for enum

		virtual const Ref<VertexBuffer>& GetVertexBuffer(unsigned int index) const = 0;

		virtual const unsigned int GetDrawingMode() const = 0;

		virtual const Engine::Ref<IndexBuffer>& GetIndexBuffer() = 0;

		static Ref<VertexArray> Create();
	};
}