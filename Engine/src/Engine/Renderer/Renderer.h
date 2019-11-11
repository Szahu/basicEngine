#pragma once

#include "RenderCommand.h"

#include "Engine/Toolbox/PerspectiveCamera.h"
#include "Engine/Renderer/Shader.h"

#include "Engine/Renderer/Model.h"
#include "Engine/Renderer/Lights.h"

#include "Engine/Renderer/FrameBuffer.h"

#include "Engine/Entity/Components/MaterialComponent.h"

namespace Engine
{


	class Renderer
	{
	public:
		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(const Ref<FrameBuffer>& frameBuffer, const PerspectiveCamera& camera, const std::vector<PointLight*>& lights, ShaderLibrary* library);
		static void EndScene();

		static void Submit(const Engine::Ref<VertexArray>& vertexArray, const MaterialStruct& material, const glm::mat4& transform = glm::mat4(1.0f), bool drawOutline = false, const Engine::Ref<Shader>& shader = m_SceneData->m_ShaderLibrary->Get("Material"),  uint32_t instances = 0);
		static void Submit(Model& model, const glm::mat4& transform);

		static inline RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			Ref<FrameBuffer> m_FrameBuffer;
			const PerspectiveCamera* m_Camera;
			std::vector<PointLight*> m_Lights;
			ShaderLibrary* m_ShaderLibrary;
		};

		static SceneData* m_SceneData;
	};


}