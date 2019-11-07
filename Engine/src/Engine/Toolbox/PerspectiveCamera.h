#pragma once

#include "glm/glm.hpp"
#include "Engine/Events/MouseEvent.h"

namespace Engine
{
	class PerspectiveCamera
	{
	public:
		PerspectiveCamera(float fov, float aspectRatio);
		void SetProjection(float fov, float aspectRatio);

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		float GetRotation() const { return m_Rotation; }
		void SetRotation(const float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

		inline const glm::vec3& GetDirection() const { return m_Direction; }
		inline const glm::vec3& GetVectorRight() const { return m_VectorRight; }
		inline const glm::vec3& GetVectorUp() const { return m_VectorUp; }
		inline const void SetAngles(float horizontal, float vertical) { m_HorizontalAngle = horizontal; m_VerticalAngle = vertical; RecalculateViewMatrix(); }
		inline const glm::vec2& GetAngles() { return glm::vec2(m_HorizontalAngle, m_VerticalAngle); }

		inline const float GetFov() const { return m_FOV; }
		inline const void SetFov(float value) { m_FOV = value; SetProjection(m_FOV, m_AspectRatio); }

		const glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4 GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		void RecalculateViewMatrix();

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 m_Direction = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_VectorRight = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_VectorUp = { 0.0f, 0.0f, 0.0f };
		float m_AspectRatio = 1.0f;
		float m_FOV = 65.0f;
		float m_HorizontalAngle = 3.14f;
		float m_VerticalAngle = 0.0f;
		float m_Rotation = 0.0f;
	};
}