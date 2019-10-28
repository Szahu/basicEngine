#include "EGpch.h"
#include "Sandbox3D.h"

#include "GLFW/glfw3.h"
#include "imgui.h"

Sandbox3D::Sandbox3D()
	:Layer("Sandbox3D"), m_Camera(65.0f, 1280.0f / 720.0f)
{

}

void Sandbox3D::OnAttach()
{
	EG_INFO("Sanbox3D online");

	float CubePositions[8 * 3] =
	{
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,

		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f
	};

	unsigned int CubeIndices[6 * 6] =
	{
		0, 1, 3, 3, 1, 2, //bottom

		2, 6, 5, 5, 2, 1, //side
		1, 5, 4, 4, 1, 0, //side
		0, 4, 7, 7, 0, 3, //side
		3, 7, 6, 6, 3, 2, //side

		6, 5, 7, 7, 5, 4 //top
	};

	Engine::BufferLayout layout{
		{ Engine::ShaderDataType::Float3, "a_Positions" }
	};

	glm::vec3 translations[100];
	int index = 0;
	int offset = 4;
	for (int x = 0; x < 10; x++)
	{
		for (int z = 0; z < 10; z++)
		{
			glm::vec3 translation(x * offset, 0.0f, z * offset);
			translations[index++] = translation;
		}
	}

	Engine::BufferLayout TranslationsLayout{
		{ Engine::ShaderDataType::Float3, "a_Translations" }
	};

	float positions[3 * 3] =
	{
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		 0.0f, -1.0f, -1.0f,
	};

	unsigned int indices[3] =
	{
		0, 1 ,2
	};

	Engine::Ref<Engine::VertexBuffer> t_VertexBuffer = Engine::VertexBuffer::Create(CubePositions, sizeof(CubePositions));
	t_VertexBuffer->SetLayout(layout);
	Engine::Ref<Engine::VertexBuffer> t_TranslationsBuffer = Engine::VertexBuffer::Create(&translations[0].x, sizeof(translations));
	t_TranslationsBuffer->SetLayout(TranslationsLayout);
	Engine::Ref<Engine::IndexBuffer> t_IndexBuffer = Engine::IndexBuffer::Create(CubeIndices, sizeof(CubeIndices) / sizeof(unsigned int));
	m_VertexArray = Engine::VertexArray::Create();
	m_VertexArray->AddVertexBuffer(t_VertexBuffer);
	m_VertexArray->AddVertexBuffer(t_TranslationsBuffer, true);
	m_VertexArray->SetIndexBuffer(t_IndexBuffer);
	m_Shader = Engine::Shader::Create("assets/shaders/flatColor.glsl");
	m_Shader->Bind();
	m_Shader->SetFloat4("u_Color", glm::vec4(m_CubeColor, 1.0f));
	m_Shader->SetMat4("u_Transform", glm::mat4(1.0f));
}

void Sandbox3D::OnDetach()
{

}

void Sandbox3D::OnUpdate(Engine::Timestep ts)
{
	Engine::RenderCommand::Clear();
	Engine::RenderCommand::SetClearColor({ 0.06f, 0.06f, 0.06f, 1.0f });

	Engine::Renderer::BeginScene(m_Camera);

	//Camera
	if (Engine::Input::IsKeyPressed(EG_KEY_W))
		m_CameraPosition += m_Camera.GetDirection() * m_CameraMoveSpeed * ts.GetSeconds();

	if (Engine::Input::IsKeyPressed(EG_KEY_S))
		m_CameraPosition -= m_Camera.GetDirection() * m_CameraMoveSpeed * ts.GetSeconds();

	if (Engine::Input::IsKeyPressed(EG_KEY_A))
		m_CameraPosition -= m_Camera.GetVectorRight() * m_CameraMoveSpeed * ts.GetSeconds();

	if (Engine::Input::IsKeyPressed(EG_KEY_D))
		m_CameraPosition += m_Camera.GetVectorRight() * m_CameraMoveSpeed * ts.GetSeconds();

	if (Engine::Input::IsKeyPressed(EG_KEY_LEFT_SHIFT))
		m_CameraPosition -= m_Camera.GetVectorUp() * m_CameraMoveSpeed * ts.GetSeconds();

	if (Engine::Input::IsKeyPressed(EG_KEY_SPACE))
		m_CameraPosition += m_Camera.GetVectorUp() * m_CameraMoveSpeed * ts.GetSeconds();

	m_Camera.SetPosition(m_CameraPosition);

	if (Engine::Input::IsMouseButtonPressed(EG_MOUSE_BUTTON_4))
	{
		float xpos = Engine::Input::GetMouseX();
		float ypos = Engine::Input::GetMouseY();
		Engine::Application::Get().GetWindow().SetCursorPosition(1280.0f / 2.0f, 720.0f / 2.0f);
		float horizontalAngle = m_Camera.GetAngles().x;
		float verticalAngle = m_Camera.GetAngles().y;
		m_Angles.x = horizontalAngle + (m_MouseSpeed * ts * float(1280.0f / 2.0f - xpos));
		m_Angles.y = verticalAngle + (m_MouseSpeed * ts * float(720.0f / 2.0f - ypos));
		m_Camera.SetAngles(m_Angles.x, m_Angles.y);
	}
	
	
	m_Shader->Bind();
	m_Shader->SetFloat4("u_Color", glm::vec4(m_CubeColor, 1.0f));
	Engine::Renderer::Submit(m_VertexArray, m_Shader, glm::mat4(1.0f));

	Engine::Renderer::EndScene();
}

void Sandbox3D::OnImGuiRender()
{
	ImGui::ColorPicker3("Cube color", &m_CubeColor.x);
}

void Sandbox3D::OnEvent(Engine::Event& event)
{
	Engine::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Engine::MouseButtonPressedEvent>(EG_BIND_EVENT_FN(Sandbox3D::OnMouseButton));
}

bool Sandbox3D::OnMouseButton(Engine::MouseButtonPressedEvent& e)
{
	if(e.GetMouseButton() == EG_MOUSE_BUTTON_4)
		Engine::Application::Get().GetWindow().SetCursorPosition(1280.0f / 2.0f, 720.0f / 2.0f);

	return false;
}
