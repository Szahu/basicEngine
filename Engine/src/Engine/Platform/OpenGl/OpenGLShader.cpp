#include "EGpch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>
#include <fstream>
#include "glm/gtc/type_ptr.hpp"

namespace Engine
{
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex") return GL_VERTEX_SHADER;
		if (type == "fragment") return GL_FRAGMENT_SHADER;
		if (type == "pixel") return GL_FRAGMENT_SHADER;
		if (type == "geometry") return GL_GEOMETRY_SHADER;

		EG_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
		:m_Path(filepath)
	{
		std::string source = ReadFile(filepath);
		auto shaderSouces = PreProcess(source);
		Compile(shaderSouces);


		// get aname from filepath
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.find('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_Name(name)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		std::string result;
		

		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			EG_CORE_ERROR("Failed to open file: '{0}'", filepath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0); //Start of shader type declaration line
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
			EG_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
			std::string type = source.substr(begin, eol - begin);
			EG_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
			EG_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

			shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(std::unordered_map<GLenum, std::string>& shaderSources)
	{
		GLuint program = glCreateProgram();
		EG_CORE_ASSERT(shaderSources.size() <= 3, "Only 3 shaders in one file are supported");
		std::array<GLenum, 3> glShaderIDs;
		unsigned int glShaderIdIndex = 0;
		for (auto& kv : shaderSources)
		{
			GLenum Type = kv.first;
			std::string& source = kv.second;

			GLuint shader = glCreateShader(Type);

			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				EG_CORE_ERROR("{0}, from Path: {1}", infoLog.data(), m_Path);
				EG_CORE_ASSERT(false, "Shader compilation failure!");
				break;
			}

			glAttachShader(program, shader);
			glShaderIDs[glShaderIdIndex++] = shader;
		}

		m_RendererID = program;

		//glBindAttribLocation(m_RendererID, 0, "a_Positions");
		//glBindAttribLocation(m_RendererID, 1, "a_Normals");
		//glBindAttribLocation(m_RendererID, 2, "a_TexCoords");
		//glBindAttribLocation(m_RendererID, 3, "a_Translations");

		glLinkProgram(m_RendererID);

		GLint isLinked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int*)& isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(m_RendererID);

			for (auto id : glShaderIDs)
			{
				glDeleteShader(id);
			}

			EG_CORE_ERROR("{0}", infoLog.data());
			EG_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		for (auto id : glShaderIDs)
		{
			glDetachShader(program, id);
		}


	}

	int OpenGLShader::GetUniformLocation(const std::string& name)
	{
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
			return m_UniformLocationCache[name];

		int location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location == -1)
			EG_CORE_WARN("{0} Warning: Uniform '{1}', doesnt exist!", m_Path, name);
		m_UniformLocationCache[name] = location;
		return location;
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

	void OpenGLShader::UplaodUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLuint location = GetUniformLocation(name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UplaodUniformFloat4(const std::string& name, const glm::vec4& data)
	{
		GLuint location = GetUniformLocation(name.c_str());
		glUniform4f(location, data.x, data.y, data.z, data.w);
	}

	void OpenGLShader::UplaodUniformFloat3(const std::string& name, const glm::vec3& data)
	{
		GLuint location = GetUniformLocation(name.c_str());
		glUniform3f(location, data.x, data.y, data.z);
	}
	void OpenGLShader::UplaodUniformFloat2(const std::string& name, const glm::vec2& data)
	{
		GLuint location = GetUniformLocation(name.c_str());
		glUniform2f(location, data.x, data.y);
	}

	void OpenGLShader::UplaodUniformFloat1(const std::string& name, float data)
	{
		GLuint location = GetUniformLocation(name.c_str());
		glUniform1f(location, data);
	}

	void OpenGLShader::UplaodUniformInt1(const std::string& name, int data)
	{
		GLuint location = GetUniformLocation(name.c_str());
		glUniform1i(location, data);
	}

	void OpenGLShader::UploadUniformFloatv(const std::string& name, const float* value, uint32_t count)
	{
		GLuint location = GetUniformLocation(name.c_str());
		glUniform1fv(location, count, value);
	}

	

}


