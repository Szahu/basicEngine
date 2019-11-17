#include "EGpch.h"
#include "MeshComponent.h"

#include "Engine/Renderer/Renderer.h"

#include "Engine/Renderer/Scene.h"

namespace Engine
{
	MeshComponent::MeshComponent()
	{

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
				"Model"
			);
		}
	}
}


