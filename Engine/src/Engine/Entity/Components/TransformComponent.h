#pragma once

#include "Engine/Entity/Component.h"

#include "glm/glm.hpp"

namespace Engine
{
	struct Transform
	{
		glm::mat4 Full_Transform = glm::mat4(1.0f);
		glm::vec3 Translation = glm::vec3(0.0f);
		glm::vec3 Rotation = glm::vec3(0.0f);
		glm::vec3 Scale = glm::vec3(1.0f);

		operator glm::mat4() const { return Full_Transform; }
	};

	class TransformComponent : public Component
	{
	public:
		TransformComponent(const std::string& ParentID) { SetParentID(ParentID); }
		~TransformComponent() = default;

		void OnImGuiRender() override;
		void OnUpdate() override;
		virtual const char* GetComponentName() override { return m_Name; }

		Transform& GetTransform() { return m_Transform; }
		
		const glm::vec3& GetPosition() { return m_Transform.Translation; }
		void SetParentID(const std::string& parentID)  { m_EntityParentID = parentID; }

	private:
		std::string m_EntityParentID = "undefined";
		Transform m_Transform;
		const char* m_Name = "Transform";
	};
}