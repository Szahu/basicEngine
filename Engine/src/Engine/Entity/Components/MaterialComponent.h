#pragma once
#include "Engine/Entity/Component.h"

#include "glm/glm.hpp"
#include "Engine/Renderer/Shader.h"

namespace Engine
{
	struct MaterialStruct
	{
		glm::vec3 m_Ambient = glm::vec3(1.0f);
		glm::vec3 m_Diffuse = glm::vec3(1.0f);
		glm::vec3 m_Specular = glm::vec3(1.0f);
		float m_Shininess = 0.5f;
	};

	class MaterialComponent
	{
	public:
		MaterialStruct GetMaterial() { return m_Material; }

		void OnImGuiRender();
		void OnUpdate() {}

	private:
		MaterialStruct m_Material;
	};
}
