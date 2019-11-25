#include "EGpch.h"
#include "Component.h"

#include "Engine/Entity/Components/MeshComponent.h"
#include "Engine/Entity/Components/TransformComponent.h"
#include "Engine/Entity/Components/ModelComponent.h"
#include "Engine/Entity/Components/MaterialComponent.h"

namespace Engine
{
	Ref<Component> Engine::Component::Create(ComponentType type, const std::string& ParentID)
	{
		switch (type)
		{
			case ComponentType::None: EG_CORE_ASSERT(false, "Component of type None is not supported!");  return nullptr;
			case ComponentType::Model:  return  std::make_shared<ModelComponent>(ParentID);
			case ComponentType::Transform: return std::make_shared<TransformComponent>(ParentID);
			case ComponentType::Mesh:  return  std::make_shared<MeshComponent>(ParentID);
			case ComponentType::Material:  return  std::make_shared<MaterialComponent>(ParentID);
		}

		return nullptr;
	}
}


