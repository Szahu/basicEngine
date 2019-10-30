#pragma once

#include "Engine.h"
#include "Engine/Core/Layer.h"


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

private:
	Engine::Ref<Engine::VertexArray> m_VertexArray;
	Engine::Ref<Engine::VertexArray> m_LampVertexArray;
	Engine::PointLight m_Light;
	glm::vec3 m_LampPosition = { 2.0f, 0.0f, 0.0f };
	Engine::Ref<Engine::Shader> m_Shader;
	Engine::Ref<Engine::Shader> m_MatShader;
	Engine::Ref<Engine::Shader> m_FlatColorShader;
	Engine::Ref<Engine::Texture2D> m_DirtTexture;
	Engine::PerspectiveCameraController m_CameraController;
	glm::vec3 m_CubeColor = { 0.3f, 0.7f, 0.5f };

	Engine::Model m_Model;
	Engine::Ref<Engine::Shader> m_ModelShader;

	int renderCount = 1;

	unsigned int FPS = 0;

	glm::vec3 amcol;
	glm::vec3 diffcol;
	glm::vec3 speccol;
};