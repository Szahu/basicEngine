#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Renderer/FrameBuffer.h"

namespace Engine
{
	class Gui
	{
	public:
		static const void InitDocspace();

		static const void ViewportWindow(const Ref<FrameBuffer>& frameBuffer);
	};
}