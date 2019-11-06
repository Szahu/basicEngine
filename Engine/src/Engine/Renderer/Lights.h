#pragma once

#include "glm/glm.hpp"

namespace Engine
{
	class PointLight
	{
	public:
		PointLight()
			:m_Position(glm::vec3(0.0f)), m_Ambient(glm::vec3(0.05f)), m_Diffuse(glm::vec3(1.0f)), m_Specular(glm::vec3(1.0f))
		{

		}

		inline const glm::vec3& GetPosition() const { return m_Position; }
		inline const glm::vec3& GetAmbient() const  { return m_Ambient; }
		inline const glm::vec3& GetDiffuse() const  { return m_Diffuse; }
		inline const glm::vec3& GetSpecular() const { return m_Specular; }

		inline const void SetPosition(glm::vec3 value) { m_Position = value; }
		inline const void SetAmbient(glm::vec3 value)  { m_Ambient = value; }
		inline const void SetDiffuse(glm::vec3 value)  { m_Diffuse = value; }
		inline const void SetSpecular(glm::vec3 value) { m_Specular = value; }

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Ambient;
		glm::vec3 m_Diffuse;
		glm::vec3 m_Specular;
	};
}