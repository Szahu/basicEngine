#pragma once

#include "RenderCommand.h"

#include "Engine/Renderer/OrtographicCamera.h"
#include "Engine./Renderer/Shader.h"

namespace Engine
{
	class Renderer
	{
	public:
		static void BeginScene(OrtographicCamera& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader);

		static inline RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};


}