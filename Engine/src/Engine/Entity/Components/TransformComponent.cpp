#include "EGpch.h"
#include "TransformComponent.h"

#include "imgui.h"

namespace Engine
{
	void Transform::Translate(const glm::vec3& vec) { m_Position += vec; }
	void Transform::Scale(const glm::vec3& vec) { m_Scale *= vec; m_CenterOfGeometry *= vec; }
	void Transform::Rotate(const glm::vec3& vec)
	{
		m_Rotation += vec;
		if (m_Rotation.x > 360) m_Rotation.x -= 360.0f;
		if (m_Rotation.y > 360) m_Rotation.y -= 360.0f;
		if (m_Rotation.z > 360) m_Rotation.z -= 360.0f;

	}

	const glm::mat4& Transform::Get()
	{

		glm::mat4 position = glm::translate(glm::mat4(1.0f), m_Position + m_CenterOfGeometry * glm::vec3(-1.0f));
		glm::mat4 RotationX = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.x), { 1, 0, 0 });
		glm::mat4 RotationY = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.y), { 0, 1, 0 });
		glm::mat4 RotationZ = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.z), { 0, 0, 1 });
		glm::mat4 RotationFULL = RotationX * RotationY * RotationZ;
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_Scale);

		Full_Transform = position * RotationFULL * scale;
		return Full_Transform;
	}

	void Transform::LookAt(const glm::vec3& dir)
	{
		glm::mat4 lookAt = glm::lookAt(m_Position, dir * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		Full_Transform *= lookAt;
	}

	void TransformComponent::OnImGuiRender()
	{
		ImGui::DragFloat3("Position", &m_Transform.GetPosition().x, 0.5f);
		ImGui::DragFloat3("Rotation", &m_Transform.GetRotation().x, 0.5f);
		ImGui::DragFloat3("Scale", &m_Transform.GetScale().x, 0.07f);
		ImGui::Separator();
	}

	void TransformComponent::OnUpdate()
	{
		//EG_CORE_ERROR("TRANSFORM COMPONENT DOESNT HAVE TO BE UPDATED!");
	}



}

