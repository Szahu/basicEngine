#pragma once

#include "Engine/Renderer/Renderer.h"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Engine
{
	class ShadowRenderer
	{
	public:

		ShadowRenderer();
		~ShadowRenderer() {}

		void PreRender(glm::vec3 casterPosition, glm::vec3 lookAtPosition);
		void PostRender(const std::initializer_list<Ref<Shader>>& elements);

		const glm::mat4& GetLightMatrix() { return lightSpaceMatrix; }
		const uint32_t GetDepthMap() { return depthMap; }
		const uint32_t GetMapSlot() { return m_DepthMapUniform; }

		void SetSize(float ortho) { Ortho = ortho; }
		void SendDataToShader(const Ref<Shader> shader);

	private:

		uint32_t depthMapFBO, depthMap;
		uint32_t m_DepthMapUniform = 20;
		unsigned int SHADOW_WIDTH = 4000, SHADOW_HEIGHT = 4000;
		float Ortho = 10.0f;
		float far_plane = 60.5f;
		float near_plane = 0.01f;
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
	};
}
