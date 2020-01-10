#pragma once

#include "Engine.h"
#include "Engine/Core/Layer.h"

#include "Level.h"
#include <thread>

using namespace Engine;

class GameLayer : public Layer
{
public:
	GameLayer();
	~GameLayer() = default;


	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Event& event) override;


private:
	unsigned int FPS = 0;
	ShaderLibrary m_ShaderLibrary;
	PerspectiveCameraController m_CameraController;
	std::vector<PointLight> m_PointLights;
	std::vector<SpotLight> m_SpotLights;
	Ref<VertexArray> m_ScreenQuad;
	Ref<FrameBuffer> m_FrameBuffer;

	SkinnedMesh testModel;
	Model testModel1;
	Tile testTile;
	Level testLevel;
};
