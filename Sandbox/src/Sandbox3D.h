#pragma once

#include "Engine.h"
#include "Engine/Core/Layer.h"

#include "glm/gtx/intersect.hpp"

#include "Engine/Renderer/Scene.h"

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
	unsigned int LoadCubemap(std::vector<std::string> faces);

private:
	Engine::ViewportWindow m_Window;

	Engine::ShaderLibrary m_ShaderLibrary;

	Engine::PointLight m_Light;
	Engine::PointLight m_Light1;
	std::vector<Engine::PointLight*> m_Lights;
	glm::vec3 m_LampPosition = { 2.0f, 0.0f, 0.0f };
	glm::vec3 m_LampPosition1 = { 0.0f, 0.0f, 0.0f };
	
	Engine::PerspectiveCameraController m_CameraController;

	int renderCount = 1;

	unsigned int FPS = 0;

	glm::vec3 amcol = {44.0f / 255.0f, 47.0f / 255.0f, 48.0f / 255.0f};
	glm::vec3 diffcol = { 1.0f, 1.0f, 1.0f };
	glm::vec3 speccol = { 213.0f / 255.0f, 225.0f / 255.0f, 44.0f / 255.0f };

	glm::vec2 frameBufferSize = { 1280, 720 };
	glm::vec3 rayPos = glm::vec3(0.0f);
	glm::vec3 rayDir = glm::vec3(0.0f);

	Engine::Scene testScene;
};