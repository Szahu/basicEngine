#pragma once

#pragma once

#include "Engine.h"
#include "Engine/Core/Layer.h"

#include "CameraControls.h"

#include "Terrain.h"

#include "Engine/Toolbox/PerlinNoise.hpp"

using namespace Engine;

class MainLayer : public Engine::Layer
{
public:
	MainLayer();
	~MainLayer() = default;


	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnDraw(Timestep ts);
	virtual void OnUpdate(Engine::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Engine::Event& event) override;


private:
	ShaderLibrary m_ShaderLibrary;
	PerspectiveCamera m_Camera;
	std::vector<PointLight> m_PointLights;
	std::vector<SpotLight> m_SpotLights;
	Ref<VertexArray> m_ScreenQuad;
	Ref<FrameBuffer> m_FrameBuffer;
private:
	CameraController m_CameraController;

	Model m_Terrain;
	Transform tr_Terrain;

	Model model;

	glm::vec2 m_Angles = glm::vec2(0.0f);
	glm::vec3 pos = glm::vec3(0.0f);

	ShadowRenderer m_Shadows;

	siv::PerlinNoise testNoise;
	Terrain testTerrain;
};