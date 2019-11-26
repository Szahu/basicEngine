#include "EGpch.h"
#include "TransformComponent.h"

#include "imgui.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Engine
{
	void TransformComponent::OnImGuiRender()
	{
		ImGui::DragFloat3("Position", &m_Transform.Translation.x, 0.5f);
		ImGui::DragFloat3("Rotation", &m_Transform.Rotation.x, 0.5f);
		ImGui::DragFloat3("Scale", &m_Transform.Scale.x, 0.07f);
		ImGui::Separator();
	}

	void TransformComponent::OnUpdate()
	{
		glm::mat4 Position = glm::translate(glm::mat4(1.0f), m_Transform.Translation);
		glm::mat4 RotationX = glm::rotate(glm::mat4(1.0f), glm::radians(m_Transform.Rotation.x), { 1, 0, 0 });
		glm::mat4 RotationY = glm::rotate(RotationX, glm::radians(m_Transform.Rotation.y), { 0, 1, 0 });
		glm::mat4 RotationFULL = glm::rotate(RotationY, glm::radians(m_Transform.Rotation.z), { 0, 0, 1 });
		glm::mat4 Scale = glm::scale(glm::mat4(1.0f), m_Transform.Scale);

		m_Transform.Full_Transform = Position * RotationFULL * Scale;
	}



}

