#pragma once

#include "Engine.h"
#include "Engine/Core/Layer.h"

#include "glm/gtx/intersect.hpp"

#include "Engine/Renderer/Scene.h"

class Sandbox3D : public Engine::Layer
{
public:
	Sandbox3D();
	~Sandbox3D() = default;


	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Engine::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Engine::Event& event) override;


private:
	Engine::ViewportWindow m_Window;
	unsigned int FPS = 0;
	Engine::Scene testScene;
};