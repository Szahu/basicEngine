#pragma once

#include "EGpch.h"

#include "Engine/Core/Core.h"
#include "Engine/Events/Event.h"

#include "Engine/Renderer/FrameBuffer.h"

#include "Engine/Core/Timestep.h"

namespace Engine
{

	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;
		Ref<FrameBuffer> FramebufferPointer;

		WindowProps(const std::string& title = "Engine Engine",
			unsigned int width = 1280,
			unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	// Interface representing a desktop system based Window
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate(Timestep ts) = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void SetCursorPosition(float x, float y) const = 0;

		virtual void* GetNativeWindow() const = 0;

		virtual void SetWindowSize(unsigned int x, unsigned int y) const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};

}
