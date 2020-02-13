#pragma once

#include "Engine.h"
#include "Engine/Core/Layer.h"

class TestLayer : public Engine::Layer
{
public:
	TestLayer() = default;
	~TestLayer() = default;


	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Engine::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Engine::Event& event) override;

private:
	unsigned int ID = 0;

};