#pragma once

#include "Engine.h"
#include "Engine/Core/Layer.h"

#include <glad/glad.h>

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
	bool OnMouseButton(Engine::MouseButtonPressedEvent& e);

private:
	Engine::Ref<Engine::VertexArray> m_VertexArray;
	Engine::Ref<Engine::Shader> m_Shader;
	Engine::PerspectiveCamera m_Camera;
	glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 4.0f };
	glm::vec2 m_Angles = glm::vec2(3.14f, 0.0f);
	glm::vec3 m_CubeColor = { 0.3f, 0.7f, 0.5f };
	float m_MouseSpeed = 1.0f;
	float m_CameraMoveSpeed = 4.0f;
};