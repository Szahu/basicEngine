#pragma once

#include "glm/glm/glm.hpp"

#include <vector>

#include "Engine.h"

struct Pillar
{
	glm::vec3 TopPosition = { 0.0f, 10.0f, 0.0f };
	glm::vec2 TopScale = { 15.0f, 20.0f};

	glm::vec3 BottomPosition = { 10.0f, 10.0f, 0.0f };
	glm::vec2 BottomScale = { 15.0f, 20.0f };
};

class Level
{
public:

	void Init();

	void OnUpdate(Engine::Timestep ts);
	void OnRender();

	void OnImGuiRender();
	void Reset();

	//Player& GetPlayer() { return m_Player; }
private:

	//Player m_Player;

	bool m_GameOver = false;

	float m_PillarTarget = 30.0f;
	int m_PillarIndex = 0;
	glm::vec3 m_PillarHSV = { 0.0f, 0.8f, 0.8f };

	std::vector<Pillar> m_Pillars;

	Engine::Ref<Engine::Texture2D> m_TriangleTexture;

};
