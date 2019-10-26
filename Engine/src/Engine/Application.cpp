#include "EGpch.h"
#include "Application.h"

#include "imgui.h"

namespace Engine
{

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
		:m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		EG_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		EG_CORE_INFO("Engine online");
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);


		//Test code

		float positions[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.7f, 0.2f, 1.0f
		};

		float positions1[4 * 7] = {
			-0.7f, -0.7f, 0.0f, 0.3f, 0.5f, 0.2f, 1.0f,
			 0.7f, -0.7f, 0.0f, 0.3f, 0.5f, 0.2f, 1.0f,
			 0.7f,  0.7f, 0.0f, 0.7f, 0.1f, 0.4f, 1.0f,
			-0.7f,  0.7f, 0.0f, 0.7f, 0.1f, 0.4f, 1.0f
		};

		uint32_t indices[3] = {
			 0, 1, 2
		};

		uint32_t indices1[6] = {
			 0, 1, 2, 2, 0, 3
		};

		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Positions"},
			{ShaderDataType::Float4, "a_Color"}
		};

		BufferLayout layout1 = {
			{ShaderDataType::Float3, "a_Positions"},
			{ShaderDataType::Float4, "a_Color"}
		};

		//Triangle
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		m_VertexBuffer.reset(VertexBuffer::Create(positions, sizeof(positions)));
		m_VertexBuffer->SetLayout(layout);
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(IndexBuffer::Create(indices, 3));
		m_VertexArray.reset(VertexArray::Create());
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
		m_VertexArray->Unbind();

		//Square
		std::shared_ptr<VertexBuffer> m_VertexBuffer2;
		m_VertexBuffer2.reset(VertexBuffer::Create(positions1, sizeof(positions1)));
		m_VertexBuffer2->SetLayout(layout1);
		std::shared_ptr<IndexBuffer> m_IndexBuffer2;
		m_IndexBuffer2.reset(IndexBuffer::Create(indices1, 6));
		m_VertexArray2.reset(VertexArray::Create());
		m_VertexArray2->AddVertexBuffer(m_VertexBuffer2);
		m_VertexArray2->SetIndexBuffer(m_IndexBuffer2);
		m_VertexArray2->Unbind();

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Positions;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjectionMatrix;

			out vec4 v_Color;
			
			void main() 
			{
				v_Color = a_Color;
				gl_Position = u_ViewProjectionMatrix * vec4(a_Positions, 1.0);
			}
								)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec4 v_Color;

			void main() 
			{
				color = v_Color;
			}
								  )";

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

	}

	Application::~Application()
	{
		
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RenderCommand::Clear();
			
			m_Camera.SetRotation(rotation);
			m_Camera.SetPosition(trnaslation);

			Renderer::BeginScene(m_Camera);

			Renderer::Submit(m_VertexArray2, m_Shader);
			Renderer::Submit(m_VertexArray, m_Shader);

			Renderer::EndScene();

			

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			ImGui::Text("HEYY");
			ImGui::SliderFloat3("translation", &trnaslation.x, -5.0f, 5.0f);
			ImGui::SliderFloat("Rotation", &rotation, 0.0f, 360.0f);
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}
