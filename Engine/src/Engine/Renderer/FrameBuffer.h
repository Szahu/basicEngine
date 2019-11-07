#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Events/ApplicationEvent.h"
#include "glm/glm.hpp"

namespace Engine
{
	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
			   
		virtual const uint32_t GetTextureID() const = 0;
		virtual void BindTexture() const = 0;
		virtual void UnbindTexture() const = 0;

		virtual void SetTextureSize(const glm::vec2& vec) = 0;
		virtual const glm::vec2& GetTextureSize() const = 0;

		virtual void OnEvent(Event& e) = 0;

		static Ref<FrameBuffer> Create();
	};
}