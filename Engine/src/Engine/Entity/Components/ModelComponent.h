#pragma once

#include "Engine/Entity/Component.h"

#include "Engine/Renderer/Model.h"

#include "Engine/Entity/Components/MaterialComponent.h"

#include "Engine/Toolbox/WindowsUtilities.h"


namespace Engine
{
	class ModelComponent : public Component
	{
	public:
		ModelComponent(const std::string& ParentID);

		void LoadModel(const std::string& path) { if(m_Model.loadModel(path)) model_loaded = true; }

		virtual void OnUpdate() override;
		virtual void OnImGuiRender() override;
		virtual const char* GetComponentName() override { return m_Name; }
		void SetParentID(const std::string& parentID) { m_EntityParentID = parentID; }


	private:
		std::string m_EntityParentID = "undefined";
		Model m_Model;
		bool model_loaded = false;
		const char* m_Name = "Model Component";
	};
}