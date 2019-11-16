#pragma once
#include "Engine/Entity/Component.h"

#include "glm/glm.hpp"
#include "Engine/Renderer/Shader.h"

#include "Engine/Renderer/Texture.h"

namespace Engine
{

	struct Material
	{
		glm::vec3 m_Ambient = glm::vec3(1.0f);
		glm::vec3 m_Diffuse = glm::vec3(1.0f);
		glm::vec3 m_Specular = glm::vec3(1.0f);
		float m_Shininess = 0.5f;
		float OverExposure = 1.0f;
		float Reflectivness = 0.0f;
		Ref<Texture2D> m_DiffuseTexture;
	};

	class MaterialComponent
	{
	public:
		//MaterialComponent() { m_Material.m_DiffuseTexture = Texture2D::Create("assets/Textures/dirt_tex.jpg"); }
		Material GetMaterial() { return m_Material; }

		void OnImGuiRender();
		void OnUpdate() {}

	private:
		Material m_Material;
	};
}
