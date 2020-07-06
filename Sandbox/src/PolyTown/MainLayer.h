#pragma once

#pragma once

#include "Engine.h"
#include "Engine/Core/Layer.h"

#include "CameraControls.h"

#include "Terrain.h"

#include "Engine/Toolbox/PerlinNoise.hpp"

#include "rp3d/src/reactphysics3d.h"

#include "Engine/Renderer/ParticleSystem.h"

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

	void CheckCollision();

private:
	ShaderLibrary* m_ShaderLibrary;
	std::vector<PointLight> m_PointLights;
	std::vector<SpotLight> m_SpotLights;
	Ref<VertexArray> m_ScreenQuad;
	Ref<FrameBuffer> m_FrameBuffer;
	Ref<FrameBuffer> m_ColorShiftBuffer;
	Ref<FrameBuffer> m_BlurBuffer;
	MousePicker m_Picker;
	CameraController m_CameraController;
private:
	ShadowRenderer m_Shadows;

	Model model;

	rp3d::CollisionWorld world;


	siv::PerlinNoise testNoise;
	Terrain testTerrain;

	double freq = 0;
	int amp = 0;
	int octave = 0;
	float spread = 0;

	float rey_length = 50.0f;
	Transform tr;
	std::thread collisionCheck;
	bool runCollisionTest = true;

	ParticleSystem m_ParticleSystem;
	ParticleProps particle;

	glm::vec3 emiterPos = glm::vec3(0.0f);
	Timer emiterTimer;
	float particleSize = 1.0f;
};