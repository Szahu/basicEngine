#include "EGpch.h"
#include "Application.h"

#include "imgui.h"

#include <GLFW/glfw3.h>

//Temporary for getting workingDir
#include <string>
#include <stdio.h>  /* defines FILENAME_MAX */
#ifdef WIN32 // enabled when compiling on windows
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

namespace Engine
{

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		EG_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		EG_CORE_INFO("Engine online");
		m_Window = Engine::Scope<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
		m_Window->SetVSync(false);

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
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
			float time = (float)glfwGetTime(); //Platform class should be
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(timestep);

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

	// Note : I use logging macros (from Hazel)
	const std::string Application::GetCurrentPath()
	{
		char cCurrentPath[FILENAME_MAX];
		if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
		{
			EG_CORE_ERROR("Couldn't Get The Current Working Dir {0}", cCurrentPath);
			return NULL;
		}

		//EG_CORE_INFO("The current working directory is : {0}", cCurrentPath);

		std::string result = cCurrentPath;
		std::replace(result.begin(), result.end(), '\\', '/');

		EG_CORE_INFO(result);
		return result;
	}

}
