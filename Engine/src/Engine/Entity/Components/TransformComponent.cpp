#include "EGpch.h"
#include "TransformComponent.h"

#include "imgui.h"

namespace Engine
{
	void TransformComponent::OnImGuiRender()
	{
		ImGui::DragFloat3("Position", &m_Transform.GetPosition().x, 0.5f);
		ImGui::DragFloat3("Rotation", &m_Transform.GetRotation().x, 0.5f);
		ImGui::DragFloat3("Scale", &m_Transform.GetScale().x, 0.07f);
		ImGui::Separator();
	}

	void TransformComponent::OnUpdate()
	{
		EG_CORE_ERROR("TRANSFORM COMPONENT DOESNT HAVE TO BE UPDATED!");
	}



}

