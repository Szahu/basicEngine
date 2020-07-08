#pragma once

#include "Engine.h"
#include "Engine/Core/Layer.h"

#include "../PolyTown/CameraControls.h"

using namespace Engine;

class DemoMainLayer : public Engine::Layer
{
public:
	DemoMainLayer();
	~DemoMainLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnDraw(Timestep ts);
	virtual void OnUpdate(Engine::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Engine::Event& event) override;


private:


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

	Model m_DemoModel;
	Ref<VertexArray> m_DemoCube;

	ShadowRenderer m_ShadowsRenderer;
};
