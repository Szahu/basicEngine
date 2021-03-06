#include "EGpch.h"
#include "OpenGLVertexArray.h"


namespace Engine
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case Engine::ShaderDataType::Float:  return GL_FLOAT;
			case Engine::ShaderDataType::Float2: return GL_FLOAT;
			case Engine::ShaderDataType::Float3: return GL_FLOAT;
			case Engine::ShaderDataType::Float4: return GL_FLOAT;
			case Engine::ShaderDataType::Mat3:   return GL_FLOAT;
			case Engine::ShaderDataType::Mat4:   return GL_FLOAT;
			case Engine::ShaderDataType::Int:    return GL_INT;
			case Engine::ShaderDataType::Int2:   return GL_INT;
			case Engine::ShaderDataType::Int3:   return GL_INT;
			case Engine::ShaderDataType::Int4:   return GL_INT;
			case Engine::ShaderDataType::Bool:   return GL_BOOL;
		}
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	void Engine::OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Engine::Ref<VertexBuffer>& vertexBuffer, bool instancing, uint32_t divisor)
	{
		EG_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "VertexBuffer has no layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		
		for (const auto& element : vertexBuffer->GetLayout())
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.GetComponenetCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalised ? GL_TRUE : GL_FALSE,
				vertexBuffer->GetLayout().GetStride(), (const void*)element.Offset); 
			if (instancing) glVertexAttribDivisor(index, divisor);
			index++;
		}

		m_BuffersSize += vertexBuffer->GetSize();
		m_VertexBuffers.push_back(vertexBuffer);
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Engine::Ref<IndexBuffer>& indexBuffer, int drawingMode)
	{
		if (drawingMode == 0) m_GlDrawingMode = GL_TRIANGLES;
		if (drawingMode == 1) m_GlDrawingMode = GL_QUADS;

		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;

		glBindVertexArray(0);
	}

}

