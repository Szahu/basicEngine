#pragma once

#include "glm/glm.hpp"
#include "Engine/Toolbox/PerspectiveCamera.h"
#include "Engine/Core/Log.h"
#include "Engine/Core/Timestep.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/KeyCodes.h"
#include "Engine/Core/Core.h"
#include "Engine/Events/MouseEvent.h"
#include "Engine/Events/ApplicationEvent.h"

#include "glm/glm/gtx/compatibility.hpp"

#include "imgui.h"

using namespace Engine;

class CameraController
{
public:
	CameraController();
	void OnUpdate(Timestep ts);
	bool OnEvent(Event& e);

	float GetPitch() const { return m_PitchAngle; }
	float GetYaw() const { return m_YawAngle; }
	float GetRoll() const { return m_RollAngle; }

	const PerspectiveCamera& GetCamera() { return m_Camera; }
	
	void OnImGuiRender();

	void SetTargetPosition(const glm::vec3& pos) { m_TargetPosition = pos; }

	void OnWindowResizeCallable(int width, int height);

private:
	
	float CalculateHorizontalDistance();
	float CalculateVerticalDistance();
	void CalculateAngleAroundTarget(Timestep ts);
	void CalculatePitchAngle(Timestep ts);
	void CalculateCameraPosition(float horizDistance, float vertDistance);
	bool OnMouseScroll(MouseScrolledEvent& e);
	bool OnWindowResize(WindowResizeEvent& e);
	void CalculateInputMovement(Timestep ts);

private:
	glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
	float m_PitchAngle = 60.0f;
	float m_YawAngle = 0.0f;
	float m_RollAngle = 0.0f;
	float m_AngleAroundTheTarget = 0.0f;
	glm::vec3 m_TargetPosition = { 0.0f, 0.0f, 0.0f };
	float m_DistanceFromTarget = 20.0f;

	float m_HorizontalSensivity = 4.0f;
	float m_VerticalSensivity = 4.0f;
	float m_MovingSensivity = 30.0f;

	glm::vec2 m_DeltaMouse = { 0.0f, 0.0f };

	float originalYaw = 0.0f;
	float originalPitch = 0.0f;
	float m_IncreaseYaw = 0.0f;
	float m_IncreasePitch = 0.0f;

	glm::vec2 prevMousePos = {0.0f, 0.0f};

	Engine::PerspectiveCamera m_Camera;
};