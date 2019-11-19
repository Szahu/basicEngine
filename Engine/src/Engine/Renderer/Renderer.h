#pragma once

#include "RenderCommand.h"

#include "Engine/Toolbox/PerspectiveCamera.h"
#include "Engine/Renderer/Shader.h"

#include "Engine/Renderer/Model.h"
#include "Engine/Renderer/Lights.h"

#include "Engine/Renderer/FrameBuffer.h"

#include "Engine/Entity/Components/MaterialComponent.h"
#include "Engine/Entity/Components/TransformComponent.h"

namespace Engine
{


	class Renderer
	{
	public:
		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void InitScene();

		static void BeginScene();
		static void EndScene();

		static void Submit(const Engine::Ref<VertexArray>& vertexArray, const Material& material, const Transform& transform, bool drawOutline, const std::string& libKey,  uint32_t instances = 0);
		static void Submit(Model& model, const Material& material, const Transform& transform, bool drawOutline, const std::string& libKey);

		static void ProcessMaterial(const Material& material, const Ref<Shader>& shader);

		static inline RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			//const Ref<FrameBuffer>& m_FrameBuffer;
			const PerspectiveCamera* m_Camera;
			std::vector<PointLight*> m_Lights;
			ShaderLibrary* m_ShaderLibrary;
			std::vector<std::string> m_ShadersInUse;
			Ref<UniformBuffer> m_LightsUniformBuffer;
			Ref<UniformBuffer> m_MatricesUniformBuffer;
			Ref<Texture2D> WhiteTexture;
		};

		static SceneData* m_SceneData;
	};


}