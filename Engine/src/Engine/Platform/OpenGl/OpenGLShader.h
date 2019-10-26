#pragma once

#include "Engine/Renderer/Shader.h"

#include "glm/glm.hpp"


namespace Engine
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string vertexSrc, const std::string fragmentSrc);
		~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void UplaodUniformMat4(const std::string& name, const glm::mat4 matrix);
		void UplaodUniformFloat4(const std::string& name, const glm::vec4 data);
		void UplaodUniformFloat3(const std::string& name, const glm::vec3 data);
		void UplaodUniformFloat2(const std::string& name, const glm::vec2 data);
		void UplaodUniformFloat1(const std::string& name, float data);
		void UplaodUniformInt1(const std::string& name, int data);

	private:
		uint32_t m_RendererID;
	};
}