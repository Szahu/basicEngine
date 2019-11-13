#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Entity/Component.h"

#include "Engine/Entity/Components/TransformComponent.h"
#include "Engine/Entity/Components/MeshComponent.h"
#include "Engine/Entity/Components/ModelComponent.h"

#include "Engine/Toolbox/MousePicker.h"

namespace Engine
{
	class Entity
	{
	public:
		Entity(std::string name = "Unnamed Entity") { m_EntityName = name; AddComponent(ComponentType::Transform); }

	
		void AddComponent(ComponentType component);

		const Ref<Component> GetComponent(ComponentType type); //Has to be dynamicly cast afterwards

		const Ref<TransformComponent> GetTransformComponent() { auto pm = std::dynamic_pointer_cast<TransformComponent>(m_Components[ComponentType::Transform]); if (pm) return pm; else EG_CORE_ERROR("Casting went wrong In GetTransformComponent"); }
		const Ref<MeshComponent> GetMeshComponent() { auto pm = std::dynamic_pointer_cast<MeshComponent>(m_Components[ComponentType::Mesh]); if (pm) return pm; else EG_CORE_ERROR("Casting went wrong In GetMeshComponent"); }
		const Ref<ModelComponent> GetModelComponent() { auto pm = std::dynamic_pointer_cast<ModelComponent>(m_Components[ComponentType::Model]); if (pm) return pm; else EG_CORE_ERROR("Casting went wrong In GetModelComponent"); }

		void OnImGuiRender();
		void OnUpdate();

		bool HasComponent(ComponentType type);

		bool CheckForIntersection(MousePicker* picker);

		void CheckIfActive(Entity* ptr) { if (ptr == this) isActive = true; else isActive = false; }
		bool IsActive() { return isActive; }

		std::string GetName() { return m_EntityName; }


	private:
		std::unordered_map<ComponentType, Ref<Component>> m_Components;
		std::string m_EntityName;
		float pickingDistance = 0.0f;
		bool isActive = false;
	};
}
