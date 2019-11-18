#include "EGpch.h"
#include "Lights.h"

#include "imgui.h"

namespace Engine
{
	void PointLight::OnImGuiRender()
	{
		ImGui::DragFloat3("Position", &m_Data.Position.x, 0.5f);
		ImGui::ColorEdit3("Ambient", &m_Data.Ambient.x);
		ImGui::ColorEdit3("Diffuse", &m_Data.Diffuse.x);
		ImGui::ColorEdit3("Specular", &m_Data.Specular.x);
	}
}


