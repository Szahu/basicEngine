#include "CameraControls.h"

using namespace Engine;

CameraController::CameraController():
	m_Camera(65.0f, 1.6f)
{

}

void CameraController::OnUpdate(Timestep ts)
{
	CalculateAngleAroundTarget(ts);
	CalculatePitchAngle();
	float horizontalDistance = CalculateHorizontalDistance();
	float verticalDistance = CalculateVerticalDistance();
	CalculateCameraPosition(horizontalDistance, verticalDistance);
	m_Camera.SetPosition(m_Position);
	m_YawAngle = 180.0f - m_AngleAroundTheTarget;
	m_Camera.SetAngles(glm::radians(m_YawAngle * -1.0f), glm::radians(360.0f - m_PitchAngle));
	m_Camera.SetDirection(glm::vec3(m_TargetPosition - m_Position));

}

bool CameraController::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<MouseScrolledEvent>(EG_BIND_EVENT_FN(CameraController::OnMouseScroll));

	return false;
}

void CameraController::OnImGuiRender()
{
	//ImGui::SliderFloat("Pitch ", &m_PitchAngle, -360, 360);
	//ImGui::SliderFloat("Yaw ", &m_YawAngle, -360, 360);
}

float CameraController::CalculateHorizontalDistance()
{
	return cos(glm::radians(m_PitchAngle)) * m_DistanceFromTarget;
}

float CameraController::CalculateVerticalDistance()
{
	return sin(glm::radians(m_PitchAngle)) * m_DistanceFromTarget;
}

void CameraController::CalculateAngleAroundTarget(Timestep ts)
{

	if (Input::IsKeyPressed(EG_KEY_LEFT))
	{
		original = m_AngleAroundTheTarget;
		m_Increase = 10.0f * ts * 100.0f * m_HorizontalSensivity;
	}
	if (Input::IsKeyPressed(EG_KEY_RIGHT))
	{
		original = m_AngleAroundTheTarget;
		m_Increase = -10.0f * ts * 100.0f * m_HorizontalSensivity;
	}

	if (original + m_Increase != m_AngleAroundTheTarget)
		m_AngleAroundTheTarget = glm::lerp(m_AngleAroundTheTarget, original + m_Increase, 0.03f);

	else if (original + m_Increase == m_AngleAroundTheTarget)
	{
		m_Increase = 0.0f;
	}
}

void CameraController::CalculatePitchAngle()
{
	if (Input::IsKeyPressed(EG_KEY_UP)) m_PitchAngle -= 1.0f;
	if (Input::IsKeyPressed(EG_KEY_DOWN)) m_PitchAngle += 1.0f;
}

void CameraController::CalculateCameraPosition(float horizDistance, float vertDistance)
{
	float xOffset = sin(glm::radians(m_AngleAroundTheTarget)) * horizDistance;
	float yOffset = vertDistance;
	float zOffset = cos(glm::radians(m_AngleAroundTheTarget)) * horizDistance;

	m_Position.x = m_TargetPosition.x + xOffset;
	m_Position.y = m_TargetPosition.y + yOffset;
	m_Position.z = m_TargetPosition.z + zOffset;
}

bool CameraController::OnMouseScroll(MouseScrolledEvent& e)
{
	float offset = e.GetYOffset();

	m_DistanceFromTarget -= offset;

	return false;
}
