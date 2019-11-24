#include "EGpch.h"
#include "Sandbox2D.h"

Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"),
	m_CameraController(1280.0f / 720.0f, true)
{
	dirt_tex = Engine::Texture2D::Create("assets/textures/dirt_tex.jpg");
}

void Sandbox2D::OnAttach()
{		
	float positions[4 * 7] = {
		-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
		 0.5f,  0.5f, 0.0f, 0.8f, 0.7f, 0.2f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.4f, 0.3f, 0.5f, 1.0f,
	};

	uint32_t indices[6] = {
		 0, 1, 2, 2, 3, 0
	};

	Engine::BufferLayout layout = {
		{Engine::ShaderDataType::Float3, "a_Positions"},
		{Engine::ShaderDataType::Float4, "a_Color"}
	};

	Engine::Ref<Engine::VertexBuffer> m_VertexBuffer;
	m_VertexBuffer = Engine::VertexBuffer::Create(positions, sizeof(positions));
	m_VertexBuffer->SetLayout(layout);
	Engine::Ref<Engine::IndexBuffer> m_IndexBuffer;
	m_IndexBuffer = Engine::IndexBuffer::Create(indices, 6);
	m_VertexArray = Engine::VertexArray::Create();
	m_VertexArray->AddVertexBuffer(m_VertexBuffer);
	m_VertexArray->SetIndexBuffer(m_IndexBuffer);
	m_VertexArray->Unbind();

	m_Shader = Engine::Shader::Create("assets/shaders/2D/FlatColor.glsl");
	m_Shader->SetFloat4("u_Color", { 1.0f, 1.0f, 1.0f, 1.0f });

}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Engine::Timestep ts)
{


	Engine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Engine::RenderCommand::Clear();

	m_CameraController.OnUpdate(ts);

	Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Engine::Renderer2D::DrawQuad({1.0f, 2.0f}, glm::vec2(1.0f, 1.0f), glm::vec4(1.3f, 0.3f, 0.8f, 1.0f));
	Engine::Renderer2D::DrawQuad(pos, glm::vec2(1.0f, 1.0f), glm::vec4(0.3f, 0.3f, 0.8f, 1.0f));
	Engine::Renderer2D::DrawQuad(glm::vec3(0.0f, 0.0f, -0.1f), glm::vec2(10.0f, 10.0f), dirt_tex);


	Engine::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Debug");
	ImGui::SliderFloat3("Pos", &pos.x, -1.0f, 1.0f);
	ImGui::End();
}

void Sandbox2D::OnEvent(Engine::Event& event)
{
	m_CameraController.OnEvent(event);
}
