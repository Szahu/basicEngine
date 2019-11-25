#include "EGpch.h"
#include "Entity.h"

#include "imgui.h"

#include "glm/gtx/intersect.hpp"

#include "Application.h"

#include "Engine/Toolbox/Samples/BasicMeshes.h"

#include "Engine/Renderer/Scene.h"

namespace Engine
{
	void Entity::AddComponent(ComponentType component)
	{
		
		
		if (HasComponent(component))
			EG_CORE_ASSERT(false, "This Entity already owns this component");

		Ref<Component> new_Component = Component::Create(component, GetName());
		m_Components[component] = new_Component;	


		//if (component == ComponentType::Mesh)
		//{
		//	auto pm = std::dynamic_pointer_cast<MeshComponent>(m_Components[component]);
		//	if (pm) pm->SetTransform(&GetTransformComponent()->GetTransform());
		//	else EG_CORE_ERROR("Casting in Entity.cpp when wrong");
		//}
	}

	const Ref<Component> Entity::GetComponent(ComponentType type)
	{
		if (HasComponent(type))
		{
			
			switch (type)
			{
				case ComponentType::None: EG_CORE_ASSERT(false, "Component of type None is not supported!");  return nullptr;
				case ComponentType::Transform: return m_Components[ComponentType::Transform];
				case ComponentType::Mesh: return m_Components[ComponentType::Mesh];
				case ComponentType::Model: return m_Components[ComponentType::Model];
				case ComponentType::Material: return m_Components[ComponentType::Material];
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
		
		AddComponentPopUp();


		int i = 0;
		int node_clicked = -1;
		ImGuiTreeNodeFlags node_flags;
		for (auto& component : m_Components)
		{
			if (m_DisplayedComponent == component.second) node_flags = ImGuiTreeNodeFlags_Selected;
			else node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
			i++;
			bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, component.second->GetComponentName(), i);
			if (ImGui::IsItemClicked())
			{
				node_clicked = i;
				m_DisplayedComponent = component.second;
			}

			if (node_open)
			{
				ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
				m_DisplayedComponent->OnImGuiRender();
				ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());

				ImGui::TreePop();
			}

		}

		//for (auto com : m_Components)
		//{
		//	com.second->OnImGuiRender();
		//}
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
		bool isWorking = glm::intersectRaySphere(Scene::GetActiveScene().GetCamera().GetCamera().GetPosition(),
			picker->GetCurrentRay(),
			GetTransformComponent()->GetPosition(), 0.85f, pickingDistance);	
		return isWorking;
	}

	void Entity::AddComponentPopUp()
	{
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
				if (ImGui::Button("ModelComponent"))
				{
					AddComponent(ComponentType::Model);
					ImGui::CloseCurrentPopup(); //Ey
				}

				ImGui::EndPopup();
			}

			ImGui::EndPopup();
		}
	}


}


