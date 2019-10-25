#pragma once
#include "Engine/Renderer/VertexArray.h"

namespace Engine
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray() {}

		virtual void Bind() const  override;
		virtual void Unbind() const override;

		virtual void AddVertexBufer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBufer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

	private:
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	private:
		uint32_t m_RendererID;
	};
}