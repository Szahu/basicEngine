#pragma once

#include "glm/glm.hpp"
#include "Engine/Toolbox/PerspectiveCamera.h"
#include "Engine/GUI/ViewportWindow.h"

namespace Engine
{
	class MousePicker
	{
	public:
		MousePicker(ViewportWindow* window = nullptr);

		inline const glm::vec3& GetCurrentRay() const { return m_CurrentRay; }

		void OnUpdate(const glm::mat4& proj, const glm::mat4& view);

		const float GetPickingDistance() { return m_PickingDistance; }

	private:
		glm::vec3 CalculateMouseRay();

		glm::vec4 toEyeCoord(const glm::vec4& clipCoord);
		glm::vec2 GetNormalizedCoords(float x, float y);

	private:
		glm::vec3 m_CurrentRay = { 0.0f, 0.0f, 0.0f };
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		ViewportWindow* window;
		const float m_PickingDistance = 0;
	};
}