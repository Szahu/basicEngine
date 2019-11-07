#pragma once

#include "glm/glm.hpp"
#include "Engine/Toolbox/PerspectiveCamera.h"

namespace Engine
{
	class MousePicker
	{
	public:
		MousePicker(const PerspectiveCamera& camera);

		inline const glm::vec3& GetCurrentRay() const { return m_CurrentRay; }

		void OnUpdate();

	private:
		glm::vec3 CalculateMouseRay();

		glm::vec4 toEyeCoord(const glm::vec4& clipCoord);


	private:
		glm::vec3 m_CurrentRay = { 0.0f, 0.0f, 0.0f };
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
	};
}