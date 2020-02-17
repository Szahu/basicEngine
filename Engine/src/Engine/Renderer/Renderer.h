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

		static void InitScene(const PerspectiveCamera* camera, ShaderLibrary* lib);

		static void BeginScene();
		static void BeginScene(std::vector<PointLight> pointlights, std::vector<SpotLight> spotlights);
		static void EndScene();

		static void Submit(const Engine::Ref<VertexArray>& vertexArray, const Material& material, const glm::mat4& transform, bool drawOutline, const std::string& libKey,  uint32_t instances = 0);
		static void Submit(Model& model, const Material& material, const const glm::mat4& transform, bool drawOutline, const std::string& libKey);
		static void Submit(Model& model, const const glm::mat4& transform, const char* libkey = "Model");

		static void ProcessMaterial(const Material& material, const Ref<Shader>& shader);

		static void SetForcedShader(const char* libKey)
		{
			if (libKey != nullptr) m_SceneData->m_ForcedShader = m_SceneData->m_ShaderLibrary->Get(libKey); 
		}
		
		static void ResetForcedShader() { m_SceneData->m_ForcedShader = nullptr; }
		static const Ref<Shader> GetForcedShader() { return m_SceneData->m_ForcedShader; }

		static inline RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		static inline void AddShader(const char* name) { m_SceneData->m_ShadersInUse.push_back(name); }

		const static unsigned int MAX_NUMBER_OF_POINTLIGHTS = 5;
		const static unsigned int MAX_NUMBER_OF_SPOTLIGHTS = 5;

	private:
		struct SceneData
		{
			//const Ref<FrameBuffer>& m_FrameBuffer;
			const PerspectiveCamera* m_Camera;
			std::vector<PointLight*> m_Lights;
			ShaderLibrary* m_ShaderLibrary;
			Ref<Shader> m_ForcedShader;
			std::vector<std::string> m_ShadersInUse;
			Ref<UniformBuffer> m_LightsUniformBuffer;
			Ref<UniformBuffer> m_MatricesUniformBuffer;
			Ref<Texture2D> WhiteTexture;
		};

		static SceneData* m_SceneData;
	};

}