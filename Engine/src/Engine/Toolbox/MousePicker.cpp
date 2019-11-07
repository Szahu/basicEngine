#include "EGpch.h"
#include "MousePicker.h"

#include "Engine/Core/Input.h"
#include "Engine/Core/Application.h"

#include "glm/gtc/matrix_transform.hpp"

namespace Engine
{
	glm::vec2 GetNormalizedCoords(float x, float y)
	{
		float mousex = (2.0f * x) / Application::Get().GetWindow().GetWidth() - 1.0f;
		float mousey = -(2.0f * y) / Application::Get().GetWindow().GetHeight() - 1.0f;

		return { mousex, mousey };
	}

	

	MousePicker::MousePicker(const PerspectiveCamera& camera) :
		m_ProjectionMatrix(camera.GetProjectionMatrix()),
		m_ViewMatrix(camera.GetViewMatrix()), 
		m_CurrentRay{0.0f, 0.0f, 0.0f}
	{

	}

	void MousePicker::OnUpdate()
	{
		m_CurrentRay = CalculateMouseRay();
	}

	glm::vec3 MousePicker::CalculateMouseRay()
	{
		float mouseX = Input::GetMouseX();
		float mouseY = Input::GetMouseY();

		glm::vec2 normalisedCoords = GetNormalizedCoords(mouseX, mouseY);
		glm::vec4 clipCoords = { normalisedCoords.x, normalisedCoords.y, -1.0f, 1.0f };
		glm::vec4 eyeCoord = toEyeCoord(clipCoords);
		glm::vec4 WorldCoords = glm::inverse(m_ViewMatrix) * eyeCoord;

		glm::vec3 MouseRay = glm::normalize(glm::vec3( WorldCoords.x, WorldCoords.y, WorldCoords.z));
		return MouseRay;
	}

	glm::vec4 MousePicker::toEyeCoord(const glm::vec4& clipCoord)
	{
		glm::mat4 invertedProjectionMatrix = glm::inverse(m_ProjectionMatrix);
		glm::vec4 eyeCoords = invertedProjectionMatrix * clipCoord;

		return { eyeCoords.x, eyeCoords.y, -1.0f, 0.0f };
	}

}

