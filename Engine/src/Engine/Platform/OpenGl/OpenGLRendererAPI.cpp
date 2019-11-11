#include "EGpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Engine
{
	void OpenGLRendererAPI::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_MULTISAMPLE);

		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		float quadVertices[] = { 
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
		};

		Engine::Ref<Engine::VertexBuffer> buffer = Engine::VertexBuffer::Create(quadVertices, sizeof(quadVertices));
		buffer->SetLayout(Engine::BufferLayout{
			{Engine::ShaderDataType::Float2, "a_Positions"},
			{Engine::ShaderDataType::Float2, "a_TexCoords"},
		});

		m_ScreenQuad = Engine::VertexArray::Create();
		m_ScreenQuad->AddVertexBuffer(buffer);

		m_ScreenShader = Shader::Create("assets/shaders/screenShader.glsl");
	}

	void OpenGLRendererAPI::RenderToScreen()
	{
		m_ScreenShader->Bind();
		m_ScreenQuad->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4 color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Engine::Ref<VertexArray>& vertexArray)
	{
		glDrawElements(vertexArray->GetDrawingMode(),vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::DrawInstanced(const Engine::Ref<VertexArray>& vertexArray, uint32_t count)
	{
		glDrawElementsInstanced(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr, count);
	}
}
