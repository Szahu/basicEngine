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
		ImGui::ColorEdit3("Diffuse", &m_Material.m_Diffuse.x , ImGuiColorEditFlags_Float);
		ImGui::ColorEdit3("Ambient", &m_Material.m_Ambient.x, ImGuiColorEditFlags_Float);
		ImGui::ColorEdit3("Specular", &m_Material.m_Specular.x, ImGuiColorEditFlags_Float);
		ImGui::SliderFloat("Shininess", &m_Material.m_Shininess, 0.0f, 1.0f);

		ImGui::Text("Diffuse Texture:");
		static char path[60];
		ImGui::SameLine();
		if (ImGui::InputText(":", path, IM_ARRAYSIZE(path), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			m_Material.m_DiffuseTexture = Texture2D::Create(path);
			path[0] = 0;
		}
		ImGui::SameLine();
		if (ImGui::Button("Load"))
		{
			m_Material.m_DiffuseTexture = Texture2D::Create(path);
			path[0] = 0;
		}

		ImGui::Separator();

		ImGui::Text("Height Texture:");
		static char path1[60];
		ImGui::SameLine();
		if (ImGui::InputText(":1", path1, IM_ARRAYSIZE(path1), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			m_Material.m_HeightMap = Texture2D::Create(path1);
			path1[0] = 0;
		}
		ImGui::SameLine();
		if (ImGui::Button("Load"))
		{
			m_Material.m_HeightMap = Texture2D::Create(path1);
			path1[0] = 0;
		}

		ImGui::Separator();

		ImGui::Text("Normal Texture:");
		static char path2[60];
		ImGui::SameLine();
		if (ImGui::InputText(":2", path2, IM_ARRAYSIZE(path2), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			m_Material.m_NormalMap = Texture2D::Create(path2);
			path2[0] = 0;
		}
		ImGui::SameLine();
		if (ImGui::Button("Load"))
		{
			m_Material.m_NormalMap = Texture2D::Create(path2);
			path2[0] = 0;
		}

		ImGui::Separator();

		ImGui::Text("Ambient Texture:");
		static char path3[60];
		ImGui::SameLine();
		if (ImGui::InputText(":3", path3, IM_ARRAYSIZE(path3), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			m_Material.m_AmbientMap = Texture2D::Create(path3);
			path3[0] = 0;
		}
		ImGui::SameLine();
		if (ImGui::Button("Load"))
		{
			m_Material.m_AmbientMap = Texture2D::Create(path3);
			path3[0] = 0;
		}

		ImGui::Separator();
	}

}

