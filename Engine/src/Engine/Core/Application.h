#pragma once

#include "Core.h"

#include "Window.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/ApplicationEvent.h"
#include "LayerStack.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Renderer2D.h"

#include "Engine/GUI/ViewportWindow.h"

#include "Engine/Imgui/ImGuiLayer.h"

#include "Engine/Core/Timestep.h"

#include "Engine/Toolbox/PerspectiveCamera.h"

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
		
		const void SetViewportWindowPointer(ViewportWindow* window) { viewport_window = window; }
		ViewportWindow* GetViewportWindowPointer() { EG_CORE_ASSERT(viewport_window, "Viewport window pointer not set / not set correctly"); return viewport_window; }

		static double GetRunningTime();

		static const Timestep& GetApplicationTimeStep() { return s_Instance->timestep; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		Engine::Scope<Window> m_Window;
		ViewportWindow* viewport_window = nullptr;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;

		Timestep timestep;

		float m_LastFrameTime = 0.0f;

		static Application* s_Instance;

	};

	Application* CreateApplication();
}
