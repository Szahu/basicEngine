#pragma once
#include "Engine/Entity/Component.h"

#include "glm/glm.hpp"
#include "Engine/Renderer/Shader.h"

#include "Engine/Renderer/Texture.h"

namespace Engine
{

	struct Material
	{
		glm::vec3 m_Ambient = glm::vec3(0.3f);
		glm::vec3 m_Diffuse = glm::vec3(0.4f);
		glm::vec3 m_Specular = glm::vec3(0.4f);
		float m_Shininess = 0.2f;
		float OverExposure = 1.0f;
		float Reflectivness = 0.0f;
		Ref<Texture2D> m_DiffuseTexture;
		Ref<Texture2D> m_NormalMap;
		Ref<Texture2D> m_HeightMap;
		Ref<Texture2D> m_AmbientMap;
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
