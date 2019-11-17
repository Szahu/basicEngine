#pragma once

#include "glm/glm.hpp"

namespace Engine
{
	struct PointLight
	{
		glm::vec3 Position = glm::vec3(0.0f);
		glm::vec3 Ambient = glm::vec3(0.25f);
		glm::vec3 Diffuse = glm::vec3(1.0f);
		glm::vec3 Specular = glm::vec3(1.0f, 1.0f, 1.0f);
	};
}