#include "EGpch.h"
#include "Component.h"

#include "Engine/Entity/Components/MeshComponent.h"
#include "Engine/Entity/Components/TransformComponent.h"
#include "Engine/Entity/Components/ModelComponent.h"

namespace Engine
{
	Ref<Component> Engine::Component::Create(ComponentType type)
	{
		switch (type)
		{
			case ComponentType::None: EG_CORE_ASSERT(false, "Component of type None is not supported!");  return nullptr;
			case ComponentType::Model:  return  std::make_shared<ModelComponent>();
			//case ComponentType::Light:  return std::make_shared<OpenGLVertexArray>();
			case ComponentType::Transform: return std::make_shared<TransformComponent>();
			case ComponentType::Mesh:  return  std::make_shared<MeshComponent>();
		}

		return nullptr;
	}
}


