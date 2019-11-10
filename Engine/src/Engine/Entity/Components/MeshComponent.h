#pragma once

#include "Engine/Entity/Component.h"

#include "Engine/Renderer/VertexArray.h"

#include "glm/glm.hpp"

#include "Engine/Entity/Components/MaterialComponent.h"

namespace Engine
{
	class MeshComponent : public Component
	{
	public:
		MeshComponent();
		virtual void OnImGuiRender() override;

		void SetVertexArray(const Ref<VertexArray>& vertexarray) { m_VertexArray = vertexarray; }
		const Ref<VertexArray>& GetVertexArray() { return m_VertexArray; }

		void SetTransform(glm::mat4* transform) { m_Transform = transform; }

		void OnUpdate() override;

	private:
		Ref<VertexArray> m_VertexArray;
		glm::mat4* m_Transform = nullptr;
		MaterialComponent m_Material;
	};

}