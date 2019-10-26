#include "EGpch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>

#include "glm/gtc/type_ptr.hpp"

namespace Engine
{
	OpenGLShader::OpenGLShader(const std::string vertexSrc, const std::string fragmentSrc)
	{

		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			EG_CORE_ERROR("{0}", infoLog.data());
			EG_CORE_ASSERT(false, "Vertex Shader compilation failure!");
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			EG_CORE_ERROR("{0}", infoLog.data());
			EG_CORE_ASSERT(false, "Fragment Shader compilation failure!");
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		m_RendererID = glCreateProgram();

		// Attach our shaders to our m_RendererID
		glAttachShader(m_RendererID, vertexShader);
		glAttachShader(m_RendererID, fragmentShader);

		// Link our program
		glLinkProgram(m_RendererID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int*)& isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(m_RendererID);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			EG_CORE_ERROR("{0}", infoLog.data());
			EG_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(m_RendererID, vertexShader);
		glDetachShader(m_RendererID, fragmentShader);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::UplaodUniformMat4(const std::string& name, glm::mat4 matrix)
	{
		GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UplaodUniformFloat4(const std::string& name, glm::vec4 data)
	{
		GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, data.x, data.y, data.z, data.w);
	}

	void OpenGLShader::UplaodUniformFloat3(const std::string& name, glm::vec3 data)
	{
		GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, data.x, data.y, data.z);
	}
	void OpenGLShader::UplaodUniformFloat2(const std::string& name, glm::vec2 data)
	{
		GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, data.x, data.y);
	}

	void OpenGLShader::UplaodUniformFloat1(const std::string& name, float data)
	{
		GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, data);
	}

	void OpenGLShader::UplaodUniformInt1(const std::string& name, int data)
	{
		GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, data);
	}


}


