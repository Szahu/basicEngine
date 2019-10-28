#include "EGpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"

#include "Engine/Renderer/RenderCommand.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Engine
{

	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColorShader;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{

		s_Data = new Renderer2DStorage();

		float positions[4 * 3] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
		};

		uint32_t indices[6] = {
			 0, 1, 2, 2, 3, 0
		};

		BufferLayout layout = {
			{Engine::ShaderDataType::Float3, "a_Positions"}
		};

		Ref<VertexBuffer> m_VertexBuffer;
		m_VertexBuffer = VertexBuffer::Create(positions, sizeof(positions));
		m_VertexBuffer->SetLayout(layout);
		Ref<Engine::IndexBuffer> m_IndexBuffer;
		m_IndexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int));
		s_Data->QuadVertexArray = VertexArray::Create();
		s_Data->QuadVertexArray->AddVertexBuffer(m_VertexBuffer);
		s_Data->QuadVertexArray->SetIndexBuffer(m_IndexBuffer);
		s_Data->QuadVertexArray->Unbind();

		s_Data->FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
	}

	void Renderer2D::ShutDown()
	{

	}

	void Renderer2D::BeginScene(const OrtographicCamera& camera)
	{
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, glm::vec2& size, glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, glm::vec2& size, glm::vec4& color)
	{
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetFloat4("u_Color", color);

		glm::mat4 Transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
		s_Data->FlatColorShader->SetMat4("u_Transform", Transform);


		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

}


