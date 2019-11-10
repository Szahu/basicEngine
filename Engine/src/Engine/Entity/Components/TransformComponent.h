#pragma once

#include "Engine/Entity/Component.h"

#include "glm/glm.hpp"

namespace Engine
{
	class TransformComponent : public Component
	{
	public:
		void OnImGuiRender() override;
		void OnUpdate() override;

		glm::mat4& GetTransform() { return m_Transform; }
		
		const glm::vec3& GetPosition() { return { m_Transform[3][1], m_Transform[3][2], m_Transform[3][3] }; }

	private:
		glm::mat4 m_Transform = glm::mat4(1.0f);
		glm::vec3 m_Translation = glm::vec3(0.0f);
		glm::vec3 m_Rotation = glm::vec3(0.0f);
		glm::vec3 m_Scale = glm::vec3(0.0f);
	};
}