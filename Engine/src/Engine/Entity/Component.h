#pragma once

#include "Engine/Core/Core.h"

#define GetParentEntity Scene::GetActiveScene().GetEntity(m_EntityParentID)

namespace Engine
{
	enum class ComponentType
	{
		None = 0, Mesh, Light, Model, Transform
	};

	class Component
	{
	public:

		virtual void OnImGuiRender() = 0;
		virtual void OnUpdate() = 0;
		virtual void SetParentID(const std::string& parentID) = 0;

		static Ref<Component> Create(ComponentType type);

	private:

	};
}
