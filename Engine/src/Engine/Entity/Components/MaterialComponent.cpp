#include "EGpch.h"
#include "MaterialComponent.h"

#include "imgui.h"

namespace Engine
{
	void MaterialComponent::OnImGuiRender()
	{
		ImGui::ColorEdit3("Diffuse", &m_Material.m_Diffuse.x);
		ImGui::ColorEdit3("Ambient", &m_Material.m_Ambient.x);
		ImGui::ColorEdit3("Specular", &m_Material.m_Specular.x);
		ImGui::SliderFloat("Shininess", &m_Material.m_Shininess, 16.0f, 128.0f);
	}

}

