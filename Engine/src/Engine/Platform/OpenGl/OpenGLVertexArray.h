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

		virtual void AddVertexBuffer(const Engine::Ref<VertexBuffer>& vertexBuffer, bool instancing = false) override;
		virtual void SetIndexBuffer(const Engine::Ref<IndexBuffer>& indexBuffer) override;

		inline virtual const Engine::Ref<IndexBuffer>& GetIndexBuffer() override { return m_IndexBuffer; }


	private:
		std::vector<Engine::Ref<VertexBuffer>> m_VertexBuffers;
		Engine::Ref<IndexBuffer> m_IndexBuffer;
	private:
		uint32_t m_RendererID;
		unsigned int index = 0;
	};
}