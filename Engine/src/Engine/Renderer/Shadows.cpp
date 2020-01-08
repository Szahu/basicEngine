#include "EGpch.h"
#include "Shadows.h"

namespace Engine
{
	ShadowRenderer::ShadowRenderer()
	{
		glGenFramebuffers(1, &depthMapFBO);
		// create depth texture
		glGenTextures(1, &depthMap);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		// attach depth texture as FBO's depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	void ShadowRenderer::PreRender(glm::vec3 casterPosition, glm::vec3 lookAtPosition)
	{
		Renderer::SetForcedShader("simpleDepthShader");

		lightProjection = glm::ortho(-Ortho, Ortho, -Ortho, Ortho, near_plane, far_plane);

		//lightView = glm::lookAt(lightPos, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
		lightView = glm::lookAt(casterPosition, lookAtPosition, { 0.0f, 1.0f, 0.0f });
		lightSpaceMatrix = lightProjection * lightView;
		Renderer::GetForcedShader()->Bind();
		Renderer::GetForcedShader()->SetMat4("lightSpaceMatrix", lightSpaceMatrix);


		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		glDisable(GL_STENCIL_TEST);
		glEnable(GL_DEPTH_TEST);
		//glCullFace(GL_FRONT);

		glDisable(GL_CULL_FACE);
	}

	void ShadowRenderer::PostRender(const std::initializer_list<Ref<Shader>>& elements)
	{
		glEnable(GL_CULL_FACE);

		//glEnable(GL_STENCIL_TEST);
		glCullFace(GL_BACK); // don't forget to reset original culling face
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glBindTextureUnit(m_DepthMapUniform, depthMap);

		Renderer::ResetForcedShader();

		for (auto& element : elements)
		{
			SendDataToShader(element);
		}
	}

	void ShadowRenderer::SendDataToShader(const Ref<Shader> shader)
	{
		shader->Bind();
		shader->SetMat4("u_LightSpaceMatrix", GetLightMatrix());
		shader->SetInt1("shadowMap", 20);
	}

	
}


