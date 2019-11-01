#include "EGpch.h"
#include "PerspectiveCamera.h"

#include "glm/gtc/matrix_transform.hpp"


namespace Engine
{
	Engine::PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio)
		:m_ProjectionMatrix(glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 300.0f)),
		m_ViewMatrix(glm::lookAt(glm::vec3(4, 3, 3),
			glm::vec3(0, 0, 0),
			glm::vec3(0, 1, 0))),
		m_ViewProjectionMatrix(m_ProjectionMatrix * m_ViewMatrix), m_AspectRatio(aspectRatio)
	{

	}

	void PerspectiveCamera::SetProjection(float fov, float aspectRatio)
	{
		m_ProjectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f);
		RecalculateViewMatrix();
	}


	void PerspectiveCamera::RecalculateViewMatrix()
	{

		m_Direction = glm::vec3(
			cos(m_VerticalAngle) * sin(m_HorizontalAngle), 
			sin(m_VerticalAngle), 
			cos(m_VerticalAngle) * cos(m_HorizontalAngle));

		m_VectorRight = glm::vec3(
			sin(m_HorizontalAngle - 3.14f / 2.0f),
			0, 
			cos(m_HorizontalAngle - 3.14f / 2.0f));

		m_VectorUp = glm::cross(m_VectorRight, m_Direction);

		m_ViewMatrix = glm::lookAt(m_Position, m_Direction + m_Position, m_VectorUp);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}


}


