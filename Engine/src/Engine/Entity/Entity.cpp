#include "EGpch.h"
#include "Entity.h"


namespace Engine
{
	void Entity::AddComponent(ComponentType component)
	{
		
		
		if (HasComponent(component))
			EG_CORE_ASSERT(false, "This Entity already owns this component");

		Ref<Component> new_Component = Component::Create(component);
		m_Components[component] = new_Component;	

		if (component == ComponentType::Mesh)
			std::dynamic_pointer_cast<MeshComponent>(m_Components[component])->SetTransform(&GetTransformComponent()->GetTransform());
	}

	const Ref<Component> Entity::GetComponent(ComponentType type)
	{
		if (HasComponent(type))
		{
			
			switch (type)
			{
				case ComponentType::None: EG_CORE_ASSERT(false, "Component of type None is not supported!");  return nullptr;
				//case ComponentType::Model:  return  std::make_shared<OpenGLVertexArray>();
				//case ComponentType::Light:  return std::make_shared<OpenGLVertexArray>();
				case ComponentType::Transform: return m_Components[ComponentType::Transform];
				case ComponentType::Mesh: return m_Components[ComponentType::Mesh];
			}
	
			EG_CORE_ASSERT(false, "Unknow Component type");
			//return nullptr; 
		}
		
		else
			EG_CORE_ASSERT(false, "This entity does not have given compoentn!");
	}

	void Entity::OnImGuiRender()
	{
		for (auto com : m_Components)
		{
			com.second->OnImGuiRender();
			//std::dynamic_pointer_cast<MeshComponent>(m_Components[ComponentType::Mesh])->SetVertexArray(nullptr);
		}
	}

	void Entity::OnUpdate()
	{
		for (auto com : m_Components)
		{
			com.second->OnUpdate();
		}
	}

	bool Entity::HasComponent(ComponentType type)
	{
		for (auto com : m_Components)
		{
			if (com.first == type)
				return true;
		}

		return false;
	}


}


