#pragma once

#include "RenderCommand.h"

#include "Engine/Toolbox/PerspectiveCamera.h"
#include "Engine/Renderer/Shader.h"

#include "Engine/Renderer/Model.h"
#include "Engine/Renderer/Lights.h"

#include "Engine/Renderer/FrameBuffer.h"

#include "Engine/Entity/Components/MaterialComponent.h"
#include "Engine/Entity/Components/TransformComponent.h"

#include "Engine/Renderer/PostProcessing.h"

namespace Engine
{

	class Renderer
	{
	public:
		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void InitScene(const PerspectiveCamera* camera);

		static void BeginScene();
		static void BeginScene(std::vector<PointLight> pointlights, std::vector<SpotLight> spotlights);
		static void EndScene(const Ref<FrameBuffer>& fbo);

		static void Submit(const Engine::Ref<VertexArray>& vertexArray, const Material& material, const glm::mat4& transform, bool drawOutline, const std::string& libKey, uint32_t instances = 0);
		static void Submit(const Engine::Ref<VertexArray>& vertexArray, const glm::mat4& transform, const std::string& libKey = "Model");
		static void Submit(Model& model, const Material& material, const const glm::mat4& transform, bool drawOutline, const std::string& libKey);
		static void Submit(Model& model, const const glm::mat4& transform, const char* libkey = "Model");

		static void ProcessMaterial(const Material& material, const Ref<Shader>& shader);

		static void SetForcedShader(const char* libKey)
		{
			if (libKey != nullptr) m_SceneData->m_ForcedShader = m_SceneData->m_ShaderLibrary.Get(libKey); 
		}
		
		static void ResetForcedShader() { m_SceneData->m_ForcedShader = nullptr; }
		static const Ref<Shader> GetForcedShader() { return m_SceneData->m_ForcedShader; }

		static inline RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		static inline void AddShader(const char* name) { m_SceneData->m_ShadersInUse.push_back(name); }

		const static unsigned int MAX_NUMBER_OF_POINTLIGHTS = 5;
		const static unsigned int MAX_NUMBER_OF_SPOTLIGHTS = 5;

		static ShaderLibrary* GetShaderLibrary() { return &m_SceneData->m_ShaderLibrary; }

		//template<typename T>
		//static void AddFunction(std::function<void(T*)> func, T* object)
		//{
		//	func(object);
		//}

		template<typename T>
		static void AddPostProcessingStage(void(T::*func)(), T* object, const Ref<FrameBuffer>& fbo)
		{
			std::function<void(T*)> funcition = func;
			funcition(object);
			m_SceneData->m_LastFrameBuffer = fbo;
		}

		const static Ref<FrameBuffer>& GetLastUsedFrameBuffer() { return m_SceneData->m_LastFrameBuffer; }

		const static Ref<VertexArray>& GetScreenQuad() { return m_SceneData->m_ScreenQuad; }

		static void SetLastUsedFrameBuffer(const Ref<FrameBuffer>& fbo) { m_SceneData->m_LastFrameBuffer = fbo; }

		static void AddPostProcessingEffect(PostProcessingEffect effect) { m_SceneData->m_Effects.push_back(effect); }

	private:
		

		struct SceneData
		{
			//const Ref<FrameBuffer>& m_FrameBuffer;
			const PerspectiveCamera* m_Camera;
			std::vector<PointLight*> m_Lights;
			ShaderLibrary m_ShaderLibrary;
			Ref<Shader> m_ForcedShader;
			std::vector<std::string> m_ShadersInUse;
			Ref<UniformBuffer> m_LightsUniformBuffer;
			Ref<UniformBuffer> m_MatricesUniformBuffer;
			Ref<Texture2D> WhiteTexture;
			Ref<FrameBuffer> m_LastFrameBuffer;
			Ref<VertexArray> m_ScreenQuad;
			std::vector<PostProcessingEffect> m_Effects;
		};

		static SceneData* m_SceneData;

	};

}