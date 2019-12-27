#pragma once

#include "Engine/Renderer/Renderer.h"
#include "glad/glad.h"
#include "glm/glm.hpp"

namespace Engine
{
	class ShadowRenderer
	{
	public:

		ShadowRenderer();
		~ShadowRenderer() {}

		template <class T>
		void RenderToDepthMap(T scene)
		{
			Renderer::SetForcedShader("simpleDepthShader");


			glm::vec3 lightPos = scene.GetSpotLights()[0].GetLightData().Position;
			glm::vec3 lightDirection = scene.GetSpotLights()[0].GetLightData().Direction;
			lightProjection = glm::ortho(-Ortho, Ortho, -Ortho, Ortho, near_plane, far_plane);

			//lightView = glm::lookAt(lightPos, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
			lightView = glm::lookAt(lightPos, lightPos + lightDirection, { 0.0f, 1.0f, 0.0f });
			lightSpaceMatrix = lightProjection * lightView;
			scene.GetShaderLibrary().Get("simpleDepthShader")->Bind();
			scene.GetShaderLibrary().Get("simpleDepthShader")->SetMat4("lightSpaceMatrix", lightSpaceMatrix);


			glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
			glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
			glClear(GL_DEPTH_BUFFER_BIT);

			glDisable(GL_STENCIL_TEST);
			glEnable(GL_DEPTH_TEST);
			//glCullFace(GL_FRONT);

			glDisable(GL_CULL_FACE);

			scene.RenderScene();

			glEnable(GL_CULL_FACE);

			//glEnable(GL_STENCIL_TEST);
			glCullFace(GL_BACK); // don't forget to reset original culling face
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glBindTextureUnit(m_DepthMapUniform, depthMap);

			Renderer::ResetForcedShader();

		}

		const glm::mat4& GetLightMatrix() { return lightSpaceMatrix; }
		const uint32_t GetDepthMap() { return depthMap; }
		const uint32_t GetMapSlot() { return m_DepthMapUniform; }

	private:

		uint32_t depthMapFBO, depthMap;
		uint32_t m_DepthMapUniform = 20;
		unsigned int SHADOW_WIDTH = 2000, SHADOW_HEIGHT = 2000;
		float Ortho = 10.0f;
		float far_plane = 60.5f;
		float near_plane = 0.01f;
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
	};
}
