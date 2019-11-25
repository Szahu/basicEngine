#pragma once

#include "Engine/Entity/Component.h"

#include "Engine/Renderer/VertexArray.h"

#include "glm/glm.hpp"

#include "Engine/Entity/Components/MaterialComponent.h"
#include "Engine/Entity/Components/TransformComponent.h"

#include "imgui.h"

namespace Engine
{
	class MeshComponent : public Component
	{
	public:
		MeshComponent(const std::string& ParentID);
		~MeshComponent() = default;

		virtual void OnImGuiRender() override { ImGui::Text("MeshComponent here!"); }
		virtual const char* GetComponentName() override { return m_Name; }
		virtual void OnUpdate() override;

		void SetVertexArray(const Ref<VertexArray> vertexarray) { m_VertexArray = vertexarray; }
		const Ref<VertexArray>& GetVertexArray() { return m_VertexArray; }


		bool IsActive = false;

		void SetParentID(const std::string& parentID) { m_EntityParentID = parentID; }
		
	private:
		Ref<VertexArray> m_VertexArray;
		std::string m_EntityParentID = "undefined";
		const char* m_Name = "MeshComponent";
	};

}