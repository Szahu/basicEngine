#include "EGpch.h"
#include "Application.h"

#include <glad/glad.h>

#include "Input.h"



namespace Engine
{

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

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

	Application::Application()
	{
		EG_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		EG_CORE_INFO("Engine online");
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);


		//Test code
		glGenVertexArrays(1, &va);
		glBindVertexArray(va);

		float positions[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.7f, 0.2f, 1.0f
		};

		uint32_t indices[3] = {
			 0, 1, 2
		};

		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Positions"},
			{ShaderDataType::Float4, "a_Color"}
		};

		//Vertex Buffer
		m_VertexBuffer.reset(VertexBuffer::Create(positions, sizeof(positions)));
		m_VertexBuffer->Bind();
		m_VertexBuffer->SetLayout(layout);
		// Test code for openGl 
		
		m_IndexBuffer.reset(IndexBuffer::Create(indices, 3));
		m_IndexBuffer->Bind();

		

		unsigned int i = 0;
		for (const auto& element : m_VertexBuffer->GetLayout())
		{
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, element.GetComponenetCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalised ? GL_TRUE : GL_FALSE, 
				layout.GetStride(), (const void*)element.Offset);
			i++;
		}

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Positions;
			layout(location = 1) in vec4 a_Color;

			out vec4 v_Color;
			
			void main() 
			{
				v_Color = a_Color;
				gl_Position = vec4(a_Positions, 1.0);
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
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->Bind();
			glBindVertexArray(va);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
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
