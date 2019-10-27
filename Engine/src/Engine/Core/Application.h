#pragma once

#include "Core.h"

#include "Window.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/ApplicationEvent.h"
#include "LayerStack.h"
#include "Engine/Renderer/Renderer.h"

#include "Engine/Imgui/ImGuiLayer.h"

#include "Engine/Core/Timestep.h"

namespace Engine
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }

		static const std::string GetCurrentPath();
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		Engine::Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;

		float m_LastFrameTime;

		static Application* s_Instance;

	};

	Application* CreateApplication();
}
