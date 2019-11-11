#include "EGpch.h"
#include "MaterialComponent.h"

#include "imgui.h"

namespace Engine
{
	void MaterialComponent::OnImGuiRender()
	{
		ImGui::SetWindowFontScale(1.1);
		ImGui::TextColored(ImVec4(255.0f / 255.0f, 211.0f / 255.0f, 147.0f / 255.0f, 1.0f), "Material:");
		ImGui::SetWindowFontScale(1.0f);
		ImGui::ColorEdit3("Diffuse", &m_Material.m_Diffuse.x);
		ImGui::ColorEdit3("Ambient", &m_Material.m_Ambient.x);
		ImGui::ColorEdit3("Specular", &m_Material.m_Specular.x);
		ImGui::SliderFloat("Shininess", &m_Material.m_Shininess, 16.0f, 64.0f);
		ImGui::Separator();
	}

}

