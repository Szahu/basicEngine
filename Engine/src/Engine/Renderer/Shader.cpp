#include "EGpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Engine/Platform/OpenGl/OpenGLShader.h"


namespace Engine
{
	Shader* Shader::Create(const std::string vertexSrc, const std::string fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: EG_CORE_ASSERT(false, "RendereAPI None is not supprtoed!");  return nullptr;
			case RendererAPI::API::OpenGL:  return new OpenGLShader(vertexSrc, fragmentSrc);
		}

		EG_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}


