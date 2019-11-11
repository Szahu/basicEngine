#include "EGpch.h"
#include "TransformComponent.h"

#include "imgui.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Engine
{
	void TransformComponent::OnImGuiRender()
	{
		ImGui::Text("Transform:");
		ImGui::DragFloat3("Position", &m_Transform.Translation.x, 0.5f);
		ImGui::DragFloat3("Scale", &m_Transform.Scale.x, 0.1f);
		ImGui::Separator();
	}

	void TransformComponent::OnUpdate()
	{
		m_Transform.Full_Transform = glm::translate(glm::mat4(1.0f), m_Transform.Translation) * glm::scale(glm::mat4(1.0f), m_Transform.Scale);
	}



}

