#pragma once

#include "glm/glm.hpp"
#include "Engine/Renderer/Texture.h"


namespace Engine
{
	struct PointLightData
	{
		glm::vec4 Position = { 0.0f, 0.0f, 15.0f, 1.0f };
		glm::vec4 Ambient = glm::vec4(0.25f, 0.25f, 0.25f, 1.0f);
		glm::vec4 Diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		glm::vec4 Specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	};


	class PointLight
	{
	public:
		void OnImGuiRender();
		const PointLightData& GetLightData() { return m_Data; }
	private:
		PointLightData m_Data;
	};

	struct SpotLightData
	{
		glm::vec4 Position = { 0.0f, 2.0f, 10.0f, 0.0f };
		glm::vec4 Direction = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
		glm::vec4 InnerCotOff_OuterCutOff = { glm::cos(glm::radians(12.0f)), glm::cos(glm::radians(17.5f)), 0.0f, 0.0f };
		glm::vec4 Ambient = glm::vec4(0.25f, 0.25f, 0.25f, 1.0f);
		glm::vec4 Diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		glm::vec4 Specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		glm::vec4 Constant_Linear_Quadratic = { 1.0f, 0.09f, 0.032f, 0.0f };
	};

	class SpotLight
	{
	public:
		void OnImGuiRender();
		SpotLightData& GetLightData() { return m_Data; }
	private:
		SpotLightData m_Data;
	};
}
	