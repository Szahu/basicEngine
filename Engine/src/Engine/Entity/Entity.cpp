#include "EGpch.h"
#include "Entity.h"

#include "imgui.h"

#include "glm/gtx/intersect.hpp"

#include "Application.h"

#include "Engine/Toolbox/Samples/BasicMeshes.h"

namespace Engine
{
	void Entity::AddComponent(ComponentType component)
	{
		
		
		if (HasComponent(component))
			EG_CORE_ASSERT(false, "This Entity already owns this component");

		Ref<Component> new_Component = Component::Create(component);
		m_Components[component] = new_Component;	

		if (component == ComponentType::Mesh)
		{
			auto pm = std::dynamic_pointer_cast<MeshComponent>(m_Components[component]);
			if (pm) pm->SetTransform(&GetTransformComponent()->GetTransform());
			else EG_CORE_ERROR("Casting in Entity.cpp when wrong");
			GetMeshComponent()->SetVertexArray(BasicMeshes::Cube());
		}
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

		ImGui::SetWindowFontScale(1.3);
		ImGui::TextColored(ImVec4(1.0f, 0.75f, 0.0f, 1.0f), GetName().c_str());
		ImGui::SetWindowFontScale(1.0f);
		ImGui::Separator();

		if (ImGui::IsMouseClicked(1) && ImGui::IsWindowHovered())
		{
			ImGui::OpenPopup("AddComponent");

			
		}

		
		if (ImGui::BeginPopup("AddComponent"))
		{
			if (ImGui::Button("Add component"))
			{
				ImGui::OpenPopup("Choose");
			}

			if (ImGui::BeginPopup("Choose"))
			{
				if (ImGui::Button("MeshComponent"))
				{
					AddComponent(ComponentType::Mesh);
					ImGui::CloseCurrentPopup(); //Ey
				}
				ImGui::EndPopup();
			}

			ImGui::EndPopup();
		}
		
		

		for (auto com : m_Components)
		{
			com.second->OnImGuiRender();
		}
	}

	void Entity::OnUpdate()
	{
		for (auto com : m_Components)
		{
			com.second->OnUpdate();
		}
		if(HasComponent(ComponentType::Mesh)) GetMeshComponent()->IsActive = isActive;
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

	bool Entity::CheckForIntersection(MousePicker* picker)
	{
		bool isWorking =  glm::intersectRaySphere(Application::Get().GetEditorCameraPointer()->GetPosition(),
			picker->GetCurrentRay(),
			GetTransformComponent()->GetPosition(), 0.85f, pickingDistance);	
		return isWorking;
	}


}


