#pragma once

#include "Engine/Entity/Component.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Engine
{
	class  Transform
	{
	
	public:
		
		inline void Translate(const glm::vec3& vec) { m_Position += vec; }
		inline void Scale(const glm::vec3& vec) { m_Scale += vec; }
		inline void Rotate(const glm::vec3& vec) { m_Rotation += vec; }

		inline glm::vec3& GetPosition() { return m_Position; }
		inline glm::vec3& GetRotation() { return m_Rotation; }
		inline glm::vec3& GetScale() { return m_Scale; }

		const glm::mat4& Get() const 
		{
			glm::mat4 position = glm::translate(glm::mat4(1.0f), m_Position);
			glm::mat4 RotationX = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.x), { 1, 0, 0 });
			glm::mat4 RotationY = glm::rotate(RotationX, glm::radians(m_Rotation.y), { 0, 1, 0 });
			glm::mat4 RotationFULL = glm::rotate(RotationY, glm::radians(m_Rotation.z), { 0, 0, 1 });
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_Scale);

			return  position * RotationFULL * scale;
		}

		operator glm::mat4() const { return Get(); }

	private:
		glm::mat4 Full_Transform = glm::mat4(1.0f);
		glm::vec3 m_Position = glm::vec3(0.0f);
		glm::vec3 m_Rotation = glm::vec3(0.0f);
		glm::vec3 m_Scale = glm::vec3(1.0f);
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
		
		const glm::vec3& GetPosition() { return m_Transform.GetPosition(); }
		void SetParentID(const std::string& parentID)  { m_EntityParentID = parentID; }

	private:
		std::string m_EntityParentID = "undefined";
		Transform m_Transform;
		const char* m_Name = "Transform";
	};
}