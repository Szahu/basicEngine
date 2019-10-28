#pragma once

#include "Engine.h"
#include "Engine/Core/Layer.h"

class Sandbox2D : public Engine::Layer
{
public:
	Sandbox2D();
	~Sandbox2D() = default;


	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Engine::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Engine::Event& event) override;

private:
	Engine::OrtographicCameraController m_CameraController;
	Engine::PerspectiveCamera m_PerspectiveCamera;
	Engine::Ref<Engine::Shader> m_Shader;
	Engine::Ref<Engine::VertexArray> m_VertexArray;
};