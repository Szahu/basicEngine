#pragma once

#include "glm/glm.hpp"
#include "Engine/Toolbox/PerspectiveCamera.h"
#include "Engine/Core/Log.h"
#include "Engine/Core/Timestep.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/KeyCodes.h"
#include "Engine/Core/Core.h"
#include "Engine/Events/MouseEvent.h"

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

private:
	
	float CalculateHorizontalDistance();
	float CalculateVerticalDistance();
	void CalculateAngleAroundTarget(Timestep ts);
	void CalculatePitchAngle();
	void CalculateCameraPosition(float horizDistance, float vertDistance);
	bool OnMouseScroll(MouseScrolledEvent& e);

private:
	glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
	float m_PitchAngle = 60.0f;
	float m_YawAngle = 0.0f;
	float m_RollAngle = 0.0f;
	float m_AngleAroundTheTarget = 0.0f;
	glm::vec3 m_TargetPosition = { 0.0f, 0.0f, 0.0f };
	float m_DistanceFromTarget = 20.0f;

	float m_HorizontalSensivity = 5.0f;

	float original = 0.0f;
	float m_Increase = 0.0f;

	Engine::PerspectiveCamera m_Camera;
};