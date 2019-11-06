#pragma once
#include "Engine/Renderer/VertexArray.h"

#include <glad/glad.h>

namespace Engine
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray() {}

		virtual void Bind() const  override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Engine::Ref<VertexBuffer>& vertexBuffer, bool instancing = false, uint32_t divisor = 1) override;
		virtual void SetIndexBuffer(const Engine::Ref<IndexBuffer>& indexBuffer, int drawingMode = 0) override;

		virtual const Ref<VertexBuffer>& GetVertexBuffer(unsigned int index) const override { return m_VertexBuffers[index]; }

		const GLenum GetDrawingMode() const override { return m_GlDrawingMode; }

		inline virtual const Engine::Ref<IndexBuffer>& GetIndexBuffer() override { return m_IndexBuffer; }


	private:
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Engine::Ref<IndexBuffer> m_IndexBuffer;
		GLenum m_GlDrawingMode;
	private:
		uint32_t m_RendererID;
		unsigned int index = 0;
	};
}