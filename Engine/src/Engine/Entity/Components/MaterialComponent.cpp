#include "EGpch.h"
#include "MaterialComponent.h"

#include "Engine/Toolbox/WindowsUtilities.h"

#include "imgui.h"

namespace Engine
{
	void MaterialComponent::OnImGuiRender()
	{
		ImGui::ColorEdit3("Diffuse", &m_Material.m_Diffuse.x , ImGuiColorEditFlags_Float);
		ImGui::ColorEdit3("Ambient", &m_Material.m_Ambient.x, ImGuiColorEditFlags_Float);
		ImGui::ColorEdit3("Specular", &m_Material.m_Specular.x, ImGuiColorEditFlags_Float);
		ImGui::SliderFloat("Shininess", &m_Material.m_Shininess, 0.0f, 1.0f);

		ImGui::Separator();

		if (ImGui::Button("Load Diffuse Texture"))
		{
			m_Material.m_DiffuseTexture = Texture2D::Create(WindowsUtilities::GetFileOpen());
		}

		if (m_Material.m_DiffuseTexture)
			ImGui::Image((void*)m_Material.m_DiffuseTexture->GetID(), { 100, 100 }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f));

		ImGui::Separator();

		if (ImGui::Button("Load HeightMap"))
		{
			m_Material.m_HeightMap = Texture2D::Create(WindowsUtilities::GetFileOpen());
		}

		if (m_Material.m_HeightMap)
			ImGui::Image((void*)m_Material.m_HeightMap->GetID(), { 100, 100 }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f));

		ImGui::Separator();

		if (ImGui::Button("Load NormalMap"))
		{
			m_Material.m_NormalMap = Texture2D::Create(WindowsUtilities::GetFileOpen());
		}

		if (m_Material.m_NormalMap)
			ImGui::Image((void*)m_Material.m_NormalMap->GetID(), { 100, 100 }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f));

		ImGui::Separator();

		if (ImGui::Button("Load AmbientMap"))
		{
			m_Material.m_AmbientMap = Texture2D::Create(WindowsUtilities::GetFileOpen());
		}

		if (m_Material.m_AmbientMap)
			ImGui::Image((void*)m_Material.m_AmbientMap->GetID(), { 100, 100 }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f));

		ImGui::Separator();
		if (ImGui::Button("Load SpecularMap"))
		{
			m_Material.m_SpecularMap = Texture2D::Create(WindowsUtilities::GetFileOpen());
		}

		if (m_Material.m_SpecularMap)
			ImGui::Image((void*)m_Material.m_SpecularMap->GetID(), { 100, 100 }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
	}

}

