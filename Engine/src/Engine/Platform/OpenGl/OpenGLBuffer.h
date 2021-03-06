#pragma once

#include "Engine/Renderer/Buffer.h"

namespace Engine
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const uint32_t GetSize() const override { return m_BufferSizeInBytes; }

		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
		virtual const BufferLayout GetLayout() const override { return m_Layout; }

		virtual const uint32_t GetRendererId() const override { return m_RendererID; };

	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
		uint32_t m_BufferSizeInBytes = 0;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const uint32_t GetSize() const override { return m_BufferSizeInBytes; }

		inline virtual uint32_t GetCount()  override { return m_Count; }

	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
		uint32_t m_BufferSizeInBytes = 0;
	};

	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(uint32_t size, uint32_t baseIndex);
		virtual ~OpenGLUniformBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddSubData(uint32_t offset, uint32_t size, const float* data) const override;

		virtual const uint32_t GetSize() const override { return m_BufferSizeInBytes; }

	private:
		uint32_t m_RendererID;
		uint32_t m_BufferSizeInBytes;
	};
}