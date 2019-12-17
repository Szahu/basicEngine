#include "EGpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"

#include "Engine/Renderer/RenderCommand.h"
#include "glm/gtc/matrix_transform.hpp"

#include "glad/glad.h"

namespace Engine
{

	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		s_Data = new Renderer2DStorage;
		float positions[4 * 5] = {
		//Positions:      //Texture Coordinates
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		uint32_t indices[6] = {
			 0, 1, 2, 2, 3, 0
		};

		BufferLayout layout = {
			{Engine::ShaderDataType::Float3, "a_Positions"},
			{Engine::ShaderDataType::Float2, "a_TexCoords"}
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

		s_Data->TextureShader = Shader::Create("assets/shaders/2D/Texture.glsl");
		s_Data->TextureShader->SetInt1("u_Texture", 0);

		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t WhiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&WhiteTextureData, sizeof(WhiteTextureData));
		glDisable(GL_STENCIL_TEST);
	}

	void Renderer2D::ShutDown()
	{

	}

	void Renderer2D::BeginScene(const OrtographicCamera& camera)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_Data->WhiteTexture->Bind();
		s_Data->TextureShader->SetFloat4("u_Color", color);

		glm::mat4 Transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
		s_Data->TextureShader->SetMat4("u_Transform", Transform);
		
		s_Data->QuadVertexArray->Bind();
		 RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		texture->Bind();
		s_Data->TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));

		glm::mat4 Transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
		s_Data->TextureShader->SetMat4("u_Transform", Transform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
}


