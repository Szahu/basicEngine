#include "EGpch.h"
#include "ModelComponent.h"

#include "Engine/Renderer/Renderer.h"

#include "Engine/Entity/Components/TransformComponent.h"

#include "Engine/Renderer/Scene.h"
#include "glm/glm.hpp"


namespace Engine
{
	ModelComponent::ModelComponent(const std::string& ParentID)
	{
		SetParentID(ParentID); 
		GetParentEntity->AddComponent(ComponentType::Material);
	}

	void ModelComponent::OnUpdate()
	{
		if (model_loaded)
		{
			Renderer::Submit(m_Model, GetParentEntity->GetMaterialComponent()->GetMaterial(), GetParentEntity->GetTransformComponent()->GetTransform().Get(), GetParentEntity->IsActive(), "Model");
		}
	}

	void ModelComponent::OnImGuiRender()
	{

		ImGui::SetWindowFontScale(1.1);
		ImGui::TextColored(ImVec4(255.0f / 255.0f, 211.0f / 255.0f, 147.0f / 255.0f, 1.0f), "Model:");
		ImGui::SetWindowFontScale(1.0f);

		if (!model_loaded)
		{
			if (ImGui::Button("Load Model"))
			{
				LoadModel(WindowsUtilities::GetFileOpen());
			}
		}
		
	
		ImGui::Separator();
	}

}

