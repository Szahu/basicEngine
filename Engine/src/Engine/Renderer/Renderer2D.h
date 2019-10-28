#pragma once

#include "Engine/Renderer/OrtographicCamera.h"

namespace Engine
{
	class Renderer2D
	{
	public:
		static void Init();
		static void ShutDown();

		static void BeginScene(const OrtographicCamera& camera);
		static void EndScene();

		// Primitives
		static void DrawQuad(const glm::vec2& position, glm::vec2& size, glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, glm::vec2& size, glm::vec4& color);
	};
}