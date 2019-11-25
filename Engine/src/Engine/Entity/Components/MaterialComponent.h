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
		Ref<Texture2D> m_SpecularMap;
	};

	class MaterialComponent : public Component
	{
	public:
		MaterialComponent(const std::string& ParentID) { SetParentID(ParentID); }
		~MaterialComponent() = default;

		Material& GetMaterial() { return m_Material; }

		virtual void OnImGuiRender() override;
		virtual void OnUpdate() override {}
		virtual const char* GetComponentName() override { return m_Name; }

		void SetParentID(const std::string& parentID) { m_EntityParentID = parentID; }

	private:
		Material m_Material;
		std::string m_EntityParentID = "undefined";
		const char* m_Name = "Material";
	};
}
