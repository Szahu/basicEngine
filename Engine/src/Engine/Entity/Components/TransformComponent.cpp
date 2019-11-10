#include "EGpch.h"
#include "TransformComponent.h"

#include "imgui.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Engine
{
	void TransformComponent::OnImGuiRender()
	{
		ImGui::DragFloat3("Position", &m_Translation.x);
	}

	void TransformComponent::OnUpdate()
	{
		m_Transform = glm::translate(glm::mat4(1.0f), m_Translation);
	}



}

