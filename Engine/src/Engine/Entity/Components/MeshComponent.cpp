#include "EGpch.h"
#include "MeshComponent.h"

#include "Engine/Renderer/Renderer.h"



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
		EG_CORE_ASSERT(m_Transform, "m_Transform is a nullptr in MeshComponent");
		glm::mat4 transform = *m_Transform;
		Renderer::Submit(GetVertexArray(),m_Material.GetMaterial(), transform);
	}
}


