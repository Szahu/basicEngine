#pragma once

#include "Engine/Entity/Component.h"

#include "Engine/Renderer/VertexArray.h"

#include "glm/glm.hpp"

#include "Engine/Entity/Components/MaterialComponent.h"
#include "Engine/Entity/Components/TransformComponent.h"

namespace Engine
{
	class MeshComponent : public Component
	{
	public:
		MeshComponent();
		virtual void OnImGuiRender() override;

		void SetVertexArray(const Ref<VertexArray> vertexarray) { m_VertexArray = vertexarray; }
		const Ref<VertexArray>& GetVertexArray() { return m_VertexArray; }

		void SetTransform(Transform* transform) { m_Transform = transform; }

		void OnUpdate() override;

		bool IsActive = false;

		void SetParentID(const std::string& parentID) override { m_EntityParentID = parentID; }

		Material& GetMaterial() { return m_Material.GetMaterial(); }
		
	private:
		Ref<VertexArray> m_VertexArray;
		Transform* m_Transform = nullptr;
		MaterialComponent m_Material;
		std::string m_EntityParentID = "undefined";
	};

}