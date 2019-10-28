#pragma once

#include "RenderCommand.h"

#include "Engine/Renderer/PerspectiveCamera.h"
#include "Engine/Renderer/Shader.h"

namespace Engine
{
	class Renderer
	{
	public:
		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(PerspectiveCamera& camera);
		static void EndScene();

		static void Submit(const Engine::Ref<VertexArray>& vertexArray, const Engine::Ref<Shader>& shader, const glm::mat4& transform = glm::mat4(1.0f), uint32_t instances = 0);

		static inline RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};


}