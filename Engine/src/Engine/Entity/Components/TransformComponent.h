#pragma once

#include "Engine/Entity/Component.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Engine
{
	class  Transform
	{
	
	public:
		Transform() {}
		Transform(glm::mat4 matrix) { Full_Transform = matrix; }

		void Translate(const glm::vec3& vec);
		void Scale(const glm::vec3& vec);
		void Rotate(const glm::vec3& vec);

		inline glm::vec3& GetPosition()  { return m_Position; }
		inline glm::vec3& GetRotation()  { return m_Rotation; }
		inline glm::vec3& GetScale()  { return m_Scale; }

		inline void SetPosition(const glm::vec3& pos) { m_Position = pos; }
		inline void SetRotation(const glm::vec3& rot) { m_Rotation = rot; }
		inline void SetScale(const glm::vec3& scale) { m_Scale = scale;  m_CenterOfGeometry = m_CenterOfGeometryOriginal * scale; }
	
		inline void SetPosition(float pos) { m_Position = glm::vec3(pos); }
		inline void SetRotation(float rot) { m_Rotation = glm::vec3(rot); }
		inline void SetScale(float scale) { m_Scale = glm::vec3(scale); m_CenterOfGeometry = m_CenterOfGeometryOriginal * scale; }

		inline void SetCenterOfGeometry(const glm::vec3& pos) { m_CenterOfGeometry = pos;  m_CenterOfGeometryOriginal = pos; }
		inline glm::vec3& GetCenterOfGeometry() { return m_CenterOfGeometry; }

		const glm::mat4& Get();

		void LookAt(const glm::vec3& dir);

		//operator const glm::mat4() const { return Get(); }
		//operator glm::mat4() { return Get(); }

	private:
		glm::mat4 Full_Transform = glm::mat4(1.0f);
		glm::vec3 m_Position = glm::vec3(0.0f);
		glm::vec3 m_Rotation = glm::vec3(0.0f);
		glm::vec3 m_Scale = glm::vec3(1.0f);
		glm::vec3 m_CenterOfGeometry = glm::vec3(0.0f);
		glm::vec3 m_CenterOfGeometryOriginal = glm::vec3(0.0f);
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