#include <Engine.h>
#include "imgui/imgui.h"

#include "Engine/Platform/OpenGl/OpenGLShader.h"

class ExampleLayer : public Engine::Layer
{
public:
	ExampleLayer()
		: Layer("example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		float positions[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.7f, 0.2f, 1.0f
		};

		float positions1[4 * 5] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		uint32_t indices[3] = {
			 0, 1, 2
		};

		uint32_t indices1[6] = {
			 0, 1, 2, 2, 0, 3
		};

		Engine::BufferLayout layout = {
			{Engine::ShaderDataType::Float3, "a_Positions"},
			{Engine::ShaderDataType::Float4, "a_Color"}
		};

		Engine::BufferLayout layout1 = {
			{Engine::ShaderDataType::Float3, "a_Positions"},
			{Engine::ShaderDataType::Float2, "a_TexCoord"}
		};

		// Setup for a Triangle
		Engine::Ref<Engine::VertexBuffer> m_VertexBuffer;
		m_VertexBuffer.reset(Engine::VertexBuffer::Create(positions, sizeof(positions)));
		m_VertexBuffer->SetLayout(layout);
		Engine::Ref<Engine::IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(Engine::IndexBuffer::Create(indices, 3));
		m_VertexArray.reset(Engine::VertexArray::Create());
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
		m_VertexArray->Unbind();

		// Setup for a Square
		Engine::Ref<Engine::VertexBuffer> m_VertexBuffer2;
		m_VertexBuffer2.reset(Engine::VertexBuffer::Create(positions1, sizeof(positions1)));
		m_VertexBuffer2->SetLayout(layout1);
		Engine::Ref<Engine::IndexBuffer> m_IndexBuffer2;
		m_IndexBuffer2.reset(Engine::IndexBuffer::Create(indices1, 6));
		m_VertexArray2.reset(Engine::VertexArray::Create());
		m_VertexArray2->AddVertexBuffer(m_VertexBuffer2);
		m_VertexArray2->SetIndexBuffer(m_IndexBuffer2);
		m_VertexArray2->Unbind();

		m_Shader = Engine::Shader::Create("assets/shaders/Gradient.glsl");
		m_TexShader = Engine::Shader::Create("assets/shaders/Texture.glsl");

		m_TextureDirt = Engine::Texture2D::Create("assets/Textures/dirt_tex.jpg");
		m_TextureLogo = Engine::Texture2D::Create("assets/Textures/cherno_logo.png");
	}

	void OnUpdate(Engine::Timestep ts) override
	{
		m_FPS =  1.0f / ts;

		if (Engine::Input::IsKeyPressed(EG_KEY_UP))
			camPosition.y += m_CameraSpeed * ts;

		if (Engine::Input::IsKeyPressed(EG_KEY_DOWN))
			camPosition.y -= m_CameraSpeed * ts;

		if (Engine::Input::IsKeyPressed(EG_KEY_LEFT))
			camPosition.x -= m_CameraSpeed * ts;

		if (Engine::Input::IsKeyPressed(EG_KEY_RIGHT))
			camPosition.x += m_CameraSpeed * ts;

		
		glm::mat4 transform1 = glm::translate(glm::mat4(1.0f), translation1);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		Engine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Engine::RenderCommand::Clear();

		m_Camera.SetRotation(rotation);
		m_Camera.SetPosition(camPosition);

		Engine::Renderer::BeginScene(m_Camera);

		
		for (int x = 0; x < 20; x++)
		{
			for (int y = 0; y < 20; y++)
			{
				glm::mat4 transform2 = glm::translate(glm::mat4(1.0f), glm::vec3(x * 0.11f, y * 0.11f, 0.0f)) * scale;
				if (x % 2 == 0)
					std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->UplaodUniformFloat4("u_Color", { 0.0f, 0.0f, 1.0f, 1.0f });
				else 
					std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->UplaodUniformFloat4("u_Color", { 0.0f, 1.0f, 0.0f, 1.0f });

				Engine::Renderer::Submit(m_VertexArray2, m_Shader, transform2);
			}
		}

		
		std::dynamic_pointer_cast<Engine::OpenGLShader>(m_Shader)->UplaodUniformFloat4("u_Color", m_TriangleColor);
		Engine::Renderer::Submit(m_VertexArray, m_Shader, transform1);

		m_TextureDirt->Bind();
		Engine::Renderer::Submit(m_VertexArray2, m_TexShader, glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 1.0f, 0.0f)));

		m_TextureLogo->Bind();
		Engine::Renderer::Submit(m_VertexArray2, m_TexShader, glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 1.0f, 0.0f)));


		Engine::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Text("FPS: %i", (int)m_FPS);
		ImGui::SliderFloat3("translation1", &translation1.x, -5.0f, 5.0f);
		ImGui::SliderFloat("Rotation", &rotation, 0.0f, 360.0f);
		ImGui::ColorPicker4("Triangle color", &m_TriangleColor.x);
	}

	void OnEvent(Engine::Event& event) override
	{

	}

private:
	Engine::ShaderLibrary m_ShaderLibrary;
	Engine::OrtographicCamera m_Camera; float m_CameraSpeed = 5.0f;
	glm::vec3 camPosition = glm::vec3(0.0f);
	glm::vec3 translation1 = glm::vec3(0.0f);
	float rotation = 0.0f;
	float m_FPS = 0.0f;
	glm::vec4 m_TriangleColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	Engine::Ref<Engine::VertexArray> m_VertexArray;
	Engine::Ref<Engine::Shader> m_Shader;
	Engine::Ref<Engine::Shader> m_TexShader;
	Engine::Ref<Engine::VertexArray> m_VertexArray2;
	Engine::Ref<Engine::Texture2D> m_TextureDirt;
	Engine::Ref<Engine::Texture2D> m_TextureLogo;
};

class Sandbox : public Engine::Application
{
public:
	Sandbox()
	{
		EG_INFO("Sandbox app Online");
		

		PushLayer(new ExampleLayer());
	}
	~Sandbox()
	{

	}
};

Engine::Application* Engine::CreateApplication()
{
	return new Sandbox();
}