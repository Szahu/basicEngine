#pragma once

#pragma once

#include "Engine.h"
#include "Engine/Core/Layer.h"

#include "CameraControls.h"

#include "Terrain.h"

#include "Engine/Toolbox/PerlinNoise.hpp"

#include "rp3d/src/reactphysics3d.h"


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
	std::vector<PointLight> m_PointLights;
	std::vector<SpotLight> m_SpotLights;
	Ref<VertexArray> m_ScreenQuad;
	Ref<FrameBuffer> m_FrameBuffer;
	MousePicker m_Picker;
private:
	CameraController m_CameraController;

	Model model;


	siv::PerlinNoise testNoise;
	Terrain testTerrain;

	double freq = 0;
	int amp = 0;
	int octave = 0;
	float spread = 0;

	rp3d::CollisionWorld world;
};