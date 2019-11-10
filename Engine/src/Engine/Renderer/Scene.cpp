#include "EGpch.h"
#include "Scene.h"

namespace Engine
{

	Scene::Scene()
		:m_ShaderLibrary(nullptr)
	{

	}

	void Scene::OnUpdate(Timestep ts)
	{
		Renderer::BeginScene(m_FrameBuffer, m_Camera.GetCamera(), m_Lights, m_ShaderLibrary);

		m_Camera.OnUpdate(ts);

		for (auto ent : m_Entities)
		{
			ent.second.OnUpdate();
		}

		Renderer::EndScene();
	}

	void Scene::OnImGuiRender()
	{
		for (auto ent : m_Entities)
		{
			ent.second.OnImGuiRender();
		}
	}

	void Scene::OnEvent(Event& e)
	{
		m_Camera.OnEvent(e);
	}

	void Scene::SetSceneData(const Ref<FrameBuffer>& frameBuffer, PerspectiveCameraController camera, const std::vector<PointLight*>& lights, ShaderLibrary* library)
	{
		m_FrameBuffer = frameBuffer;
		m_Camera = camera;
		m_ShaderLibrary = library;
		m_Lights = lights;
	}

	void Scene::AddEntity(Entity& entity)
	{
		for (auto em : m_Entities)
		{
			if (entity.GetName() == em.first)
			{
				EG_CORE_WARN("There already is an Entity with this name!");
				return;
			}
		}
		m_Entities[entity.GetName()] = entity;
	}

}
