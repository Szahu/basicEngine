#include "EGpch.h"
#include "MeshComponent.h"

#include "Engine/Renderer/Renderer.h"

#include "Engine/Renderer/Scene.h"
#include "Engine/Renderer/Model.h"

#include "Engine/Toolbox/Samples/BasicMeshes.h"

namespace Engine
{
	MeshComponent::MeshComponent()
	{
		//GetMeshComponent()->SetVertexArray(BasicMeshes::Cube());
		m_VertexArray = BasicMeshes::Cube();
	}

	void MeshComponent::OnImGuiRender()
	{
		m_Material.OnImGuiRender();
	}

	void MeshComponent::OnUpdate()
	{
		if (m_VertexArray)
		{
			Renderer::Submit(GetVertexArray(),
				m_Material.GetMaterial(),
				GetParentEntity->GetTransformComponent()->GetTransform(),
				GetParentEntity->IsActive(),
				"Material"
			);
		}
	}
}


