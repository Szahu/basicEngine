#pragma once

#include "glm/glm.hpp"

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
}
	