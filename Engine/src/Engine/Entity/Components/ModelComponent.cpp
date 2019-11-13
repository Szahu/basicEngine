#include "EGpch.h"
#include "ModelComponent.h"

#include "Engine/Renderer/Renderer.h"

#include "Engine/Entity/Components/TransformComponent.h"

#include "Engine/Renderer/Scene.h"
#include "glm/glm.hpp"

namespace Engine
{
	void ModelComponent::OnUpdate()
	{
		if (model_loaded)
		{
			Renderer::Submit(m_Model, m_Material.GetMaterial(), GetParentEntity->GetTransformComponent()->GetTransform(), GetParentEntity->IsActive(), "Model");
		}
	}

	void ModelComponent::OnImGuiRender()
	{
		m_Material.OnImGuiRender();

		ImGui::SetWindowFontScale(1.1);
		ImGui::TextColored(ImVec4(255.0f / 255.0f, 211.0f / 255.0f, 147.0f / 255.0f, 1.0f), "Model:");
		ImGui::SetWindowFontScale(1.0f);

		if (!model_loaded)
		{
			static char path[60];
			ImGui::InputText("Path: ", path, IM_ARRAYSIZE(path));
			ImGui::SameLine();
			if (ImGui::Button("Load Model"))
			{
				EG_CORE_INFO("Loadin a model");
				LoadModel("assets/models/" + std::string(path));
			}
		}
		
		ImGui::Separator();
	}

}
