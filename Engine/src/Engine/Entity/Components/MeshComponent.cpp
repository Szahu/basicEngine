#include "EGpch.h"
#include "MeshComponent.h"

#include "Engine/Renderer/Renderer.h"

#include "Engine/Renderer/Scene.h"
#include "Engine/Renderer/Model.h"

namespace Engine
{
	MeshComponent::MeshComponent()
	{
		//GetMeshComponent()->SetVertexArray(BasicMeshes::Cube());
		Model model;
		model.loadModel("assets/models/hdcube.obj");
		m_VertexArray = model.GetMeshes()[0].GetVertexArray();
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


