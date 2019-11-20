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

	void SpotLight::OnImGuiRender()
	{
		ImGui::DragFloat3("Position", &m_Data.Position.x, 0.5f);
		ImGui::DragFloat3("Direction", &m_Data.Direction.x, 0.5f);
		ImGui::ColorEdit3("Ambient", &m_Data.Ambient.x);
		ImGui::ColorEdit3("Diffuse", &m_Data.Diffuse.x);
		ImGui::ColorEdit3("Specular", &m_Data.Specular.x);
		ImGui::SliderFloat("Inner Cut Off", &m_Data.InnerCotOff_OuterCutOff.x, 0.0f, 5.0f);
		ImGui::SliderFloat("Outer Cut Off", &m_Data.InnerCotOff_OuterCutOff.y, 0.0f, 5.0f);
		ImGui::SliderFloat("Constant", &m_Data.Constant_Linear_Quadratic.x, 0.0f, 5.0f);
		ImGui::SliderFloat("Linear", &m_Data.Constant_Linear_Quadratic.y, 0.0f, 5.0f);
		ImGui::SliderFloat("Quadratic", &m_Data.Constant_Linear_Quadratic.z, 0.0f, 1.0f);
	}
}


