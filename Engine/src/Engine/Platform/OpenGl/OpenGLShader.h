#pragma once

#include "Engine/Renderer/Shader.h"
#include "glm/glm.hpp"

typedef unsigned int GLenum;

namespace Engine
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetMat4(const std::string& name, const glm::mat4& value) override { UplaodUniformMat4(name, value); }
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) override { UplaodUniformFloat4(name, value); }
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) override { UplaodUniformFloat3(name, value); }
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) override { UplaodUniformFloat2(name, value); }
		virtual void SetFloat1(const std::string& name, const float value) override { UplaodUniformFloat1(name, value); }

		virtual const std::string& GetName() const override { return m_Name; }

		void UplaodUniformMat4(const std::string& name, const glm::mat4& matrix);
		void UplaodUniformFloat4(const std::string& name, const glm::vec4& data);
		void UplaodUniformFloat3(const std::string& name, const glm::vec3& data);
		void UplaodUniformFloat2(const std::string& name, const glm::vec2& data);
		void UplaodUniformFloat1(const std::string& name, float data);
		void UplaodUniformInt1(const std::string& name, int data);

	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(std::unordered_map<GLenum, std::string>& shaderSources);
	private:
		std::string m_Name;
		uint32_t m_RendererID;
	};
}