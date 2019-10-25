#pragma once

#include <memory>

#include "Engine/Renderer/Buffer.h"

namespace Engine
{
	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBufer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBufer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

		static VertexArray* Create();
	};
}