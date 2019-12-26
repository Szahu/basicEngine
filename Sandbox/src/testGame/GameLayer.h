#pragma once

#include "Engine.h"
#include "Engine/Core/Layer.h"


class GameLayer : public Engine::Layer 
{
public:
	GameLayer();
	~GameLayer() = default;


	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Engine::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Engine::Event& event) override;

	void CreateCamera(uint32_t width, uint32_t height);

private:
	Engine::Scope<Engine::OrtographicCamera> m_Camera;

	float m_Time = 0.0f;
	bool m_Blink = false;

	enum class GameState
	{
		Play = 0, MainMenu = 1, GameOver = 2
	};
};