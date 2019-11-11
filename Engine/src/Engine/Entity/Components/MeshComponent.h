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

		void SetVertexArray(const Ref<VertexArray>& vertexarray) { m_VertexArray = vertexarray; }
		const Ref<VertexArray>& GetVertexArray() { return m_VertexArray; }

		void SetTransform(Transform* transform) { m_Transform = transform; }
		//void SetSelectingBool(bool* ptr) { *IsActive = ptr; }

		void OnUpdate() override;

		bool IsActive = false;


	private:
		Ref<VertexArray> m_VertexArray;
		Transform* m_Transform = nullptr;
		MaterialComponent m_Material;
	};

}