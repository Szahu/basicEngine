#include "EGpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "Engine/platform/OpenGl/imGuiOpenGlRenderer.h"
#include "Engine/Keycodes.h"

#include "Engine/Application.h"
#include "Engine/Core.h"

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

//Temporary 
#include <Glad/glad.h>

// Note : I use logging macros (from Hazel)
const std::string CurrentPath()
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
	return result;
}

namespace Engine
{
	ImGuiLayer::ImGuiLayer()
		:Layer("ImGui layer")
	{

	}

	ImGuiLayer::~ImGuiLayer()
	{

	}

	void ImGuiLayer::OnAttach()
	{

		ImGui::CreateContext();
		ImGui::StyleColorsLight();

		ImGuiIO& io = ImGui::GetIO();
		ImFont* font1 = io.Fonts->AddFontFromFileTTF((CurrentPath() + "/../Engine/src/Engine/ImGUi/Fonts/Roboto/Roboto-Medium.ttf").c_str(), 16.0f);

		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		//Temporary 
		io.KeyMap[ImGuiKey_Tab] = EG_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = EG_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = EG_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = EG_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = EG_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = EG_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = EG_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = EG_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = EG_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = EG_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = EG_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = EG_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = EG_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = EG_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = EG_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = EG_KEY_A;
		io.KeyMap[ImGuiKey_C] = EG_KEY_C;
		io.KeyMap[ImGuiKey_V] = EG_KEY_V;
		io.KeyMap[ImGuiKey_X] = EG_KEY_X;
		io.KeyMap[ImGuiKey_Y] = EG_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = EG_KEY_Z;

		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
		
	}

	void ImGuiLayer::OnUpdate()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();  


		//float time = (float)EGGetTime();
		//io.DeltaTime = m_Time > 0.0 ? (time - m_Time) : (1.0f / 60.0f);

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseButtonPressedEvent>(EG_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(EG_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(EG_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(EG_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
		dispatcher.Dispatch<KeyPressedEvent>(EG_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(EG_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(EG_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
		dispatcher.Dispatch<KeyTypedEvent>(EG_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));

	}

	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[event.GetMouseButton()] = true;

		return false;
	}

	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[event.GetMouseButton()] = false;

		return false;
	}

	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(event.GetX(), event.GetY());

		return false;
	}

	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheel += event.GetYOffset();
		io.MouseWheelH += event.GetXOffset();

		return false;
	}

	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[event.GetKeyCode()] = true;

		io.KeyCtrl = io.KeysDown[EG_KEY_LEFT_CONTROL] || io.KeysDown[EG_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[EG_KEY_LEFT_SHIFT] || io.KeysDown[EG_KEY_RIGHT_SHIFT];
		io.KeySuper = io.KeysDown[EG_KEY_LEFT_SUPER] || io.KeysDown[EG_KEY_RIGHT_SUPER];
		io.KeyAlt = io.KeysDown[EG_KEY_LEFT_ALT] || io.KeysDown[EG_KEY_RIGHT_ALT];
		return false;
	}

	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[event.GetKeyCode()] = false;

		return false;
	}

	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		int c = event.GetKeyCode();
		if (c > 0 && c < 0x10000)
			io.AddInputCharacter((unsigned short)c);

		return false;
	}

	bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(event.GetWidth(), event.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, event.GetWidth(), event.GetHeight());

		return false;
	}
}