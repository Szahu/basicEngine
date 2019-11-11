#include "EGpch.h"
#include "MousePicker.h"

#include "Engine/Core/Input.h"
#include "Engine/Core/Application.h"

#include "glm/gtc/matrix_transform.hpp"

namespace Engine
{
	glm::vec2 MousePicker::GetNormalizedCoords(float x, float y)
	{
		float mousex = (2.0f * x) / window->GetFrameBuffer()->GetTextureSize().x - 1.0f;
		float mousey = -(2.0f * y) / window->GetFrameBuffer()->GetTextureSize().y - 1.0f;

		return { mousex, mousey };
	}

	

	MousePicker::MousePicker(ViewportWindow* window) :
		m_CurrentRay{ 0.0f, 0.0f, 0.0f },
		window(window)
	{

	}

	void MousePicker::OnUpdate(const glm::mat4& proj, const glm::mat4& view)
	{
		m_ProjectionMatrix = proj;
		m_ViewMatrix = view;
		m_CurrentRay = CalculateMouseRay();
	}

	glm::vec3 MousePicker::CalculateMouseRay()
	{
		float mouse_x = window->GetRelativeMousePos().x;
		float mouse_y = window->GetRelativeMousePos().y;

		float width = window->GetFrameBuffer()->GetTextureSize().x;
		float height = window->GetFrameBuffer()->GetTextureSize().y;

		float x = (2.0f * mouse_x) / width - 1.0f;
		float y = 1.0f - (2.0f * mouse_y) / height;
		float z = 1.0f;

		glm::vec3 ray_nds = glm::vec3(x, y, z);

		glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0f, 1.0f);

		glm::vec4 ray_eye = glm::inverse(m_ProjectionMatrix) * ray_clip;

		ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);

		glm::vec3 ray_wor = (glm::inverse(m_ViewMatrix) * ray_eye);
		// don't forget to normalise the vector at some point
		ray_wor = glm::normalize(ray_wor);
		return ray_wor;
	}

	glm::vec4 MousePicker::toEyeCoord(const glm::vec4& clipCoord)
	{
		glm::mat4 invertedProjectionMatrix = glm::inverse(m_ProjectionMatrix);
		glm::vec4 eyeCoords = invertedProjectionMatrix * clipCoord;

		return { eyeCoords.x, eyeCoords.y, -1.0f, 0.0f };
	}

}

