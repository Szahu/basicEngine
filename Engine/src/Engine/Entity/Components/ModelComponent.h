#pragma once

#include "Engine/Entity/Component.h"

#include "Engine/Renderer/Model.h"

#include "Engine/Entity/Components/MaterialComponent.h"

namespace Engine
{
	class ModelComponent : public Component
	{
	public:

		void LoadModel(const std::string& path) { if(m_Model.loadModel(path)) model_loaded = true; }

		void OnUpdate() override;
		void OnImGuiRender() override;
		void SetParentID(const std::string& parentID) override { m_EntityParentID = parentID; }

	private:
		std::string m_EntityParentID = "undefined";
		Model m_Model;
		bool model_loaded = false;
		MaterialComponent m_Material;
	};
}