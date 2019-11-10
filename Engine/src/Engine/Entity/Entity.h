#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Entity/Component.h"

#include "Engine/Entity/Components/TransformComponent.h"
#include "Engine/Entity/Components/MeshComponent.h"

namespace Engine
{
	class Entity
	{
	public:
		Entity(std::string name = "Unnamed Entity") { m_EntityName = name; AddComponent(ComponentType::Transform); }

	
		void AddComponent(ComponentType component);

		const Ref<Component> GetComponent(ComponentType type); //Has to be dynamicly cast afterwards

		const Ref<TransformComponent> GetTransformComponent() { return std::dynamic_pointer_cast<TransformComponent>(m_Components[ComponentType::Transform]); }
		const Ref<MeshComponent> GetMeshComponent() { return std::dynamic_pointer_cast<MeshComponent>(m_Components[ComponentType::Mesh]); }

		void OnImGuiRender();
		void OnUpdate();

		bool HasComponent(ComponentType type);

		std::string GetName() { return m_EntityName; }

	private:
		std::unordered_map<ComponentType, Ref<Component>> m_Components;
		std::string m_EntityName;
	};
}
