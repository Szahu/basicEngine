#include "EGpch.h"
#include "MeshComponent.h"

#include "Engine/Renderer/Renderer.h"

#include "Engine/Renderer/Scene.h"
#include "Engine/Renderer/Model.h"

#include "Engine/Toolbox/Samples/BasicMeshes.h"

namespace Engine
{
	MeshComponent::MeshComponent(const std::string& ParentID)
	{
		SetParentID(ParentID);
		m_VertexArray = BasicMeshes::Cube();
		GetParentEntity->AddComponent(ComponentType::Material);
	}

	void MeshComponent::OnUpdate()
	{
		if (m_VertexArray)
		{
			Renderer::Submit(GetVertexArray(),
				GetParentEntity->GetMaterialComponent()->GetMaterial(),
				GetParentEntity->GetTransformComponent()->GetTransform(),
				GetParentEntity->IsActive(),
				"Material"
			);
		}
	}
}


