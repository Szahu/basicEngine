#include "CameraControls.h"

using namespace Engine;

CameraController::CameraController():
	m_Camera(65.0f, 1.6f)
{

}

void CameraController::OnUpdate(Timestep ts)
{
	glm::vec2 mousepos = Input::GetMousePos();
	m_DeltaMouse = mousepos - prevMousePos;


	CalculateAngleAroundTarget(ts);
	CalculatePitchAngle(ts);
	CalculateInputMovement(ts);
	float horizontalDistance = CalculateHorizontalDistance();
	float verticalDistance = CalculateVerticalDistance();
	CalculateCameraPosition(horizontalDistance, verticalDistance);
	m_Camera.SetPosition(m_Position);
	m_YawAngle = 180.0f - m_AngleAroundTheTarget;
	m_Camera.SetAngles(glm::radians(m_YawAngle * -1.0f), glm::radians(360.0f - m_PitchAngle));
	m_Camera.SetDirection(glm::vec3(m_TargetPosition - m_Position));

	prevMousePos = mousepos;
}

bool CameraController::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<MouseScrolledEvent>(EG_BIND_EVENT_FN(CameraController::OnMouseScroll));

	dispatcher.Dispatch<WindowResizeEvent>(EG_BIND_EVENT_FN(CameraController::OnWindowResize));

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
	if (Input::IsMouseButtonPressed(EG_MOUSE_BUTTON_MIDDLE) && !Input::IsKeyPressed(EG_KEY_LEFT_SHIFT))
	{
		originalYaw = m_AngleAroundTheTarget;
		m_IncreaseYaw =  -1.0f * m_DeltaMouse.x * 10.0f * ts * 100.0f * m_HorizontalSensivity;
	}


	if (m_IncreaseYaw < 0)
	{
		if (originalYaw + m_IncreaseYaw <= m_AngleAroundTheTarget)
			m_AngleAroundTheTarget = glm::lerp(m_AngleAroundTheTarget, originalYaw + m_IncreaseYaw, 0.02f);
	
		else if (originalYaw + m_IncreaseYaw > m_AngleAroundTheTarget)
		{
			m_IncreaseYaw = 0.0f;
		}
	}
	
	else if (m_IncreaseYaw > 0)
	{
		if (originalYaw + m_IncreaseYaw >= m_AngleAroundTheTarget)
			m_AngleAroundTheTarget = glm::lerp(m_AngleAroundTheTarget, originalYaw + m_IncreaseYaw, 0.02f);
	
		else if (originalYaw + m_IncreaseYaw < m_AngleAroundTheTarget)
		{
			m_IncreaseYaw = 0.0f;
		}
	}
}

void CameraController::CalculatePitchAngle(Timestep ts)
{
	if (Input::IsMouseButtonPressed(EG_MOUSE_BUTTON_MIDDLE) && !Input::IsKeyPressed(EG_KEY_LEFT_SHIFT))
	{
		originalPitch = m_PitchAngle;
		m_IncreasePitch = m_DeltaMouse.y * 10.0f * ts * 100.0f * m_VerticalSensivity;
	}
	
	
	if (m_IncreasePitch < 0)
	{
		if (originalPitch + m_IncreasePitch <= m_PitchAngle)
			m_PitchAngle = glm::lerp(m_PitchAngle, originalPitch + m_IncreasePitch, 0.02f);
	
		else if (originalPitch + m_IncreasePitch > m_PitchAngle)
		{
			m_IncreasePitch = 0.0f;
		}
	}
	
	else if (m_IncreasePitch > 0)
	{
		if (originalPitch + m_IncreasePitch >= m_PitchAngle)
			m_PitchAngle = glm::lerp(m_PitchAngle, originalPitch + m_IncreasePitch, 0.02f);
	
		else if (originalPitch + m_IncreasePitch < m_PitchAngle)
		{
			m_IncreasePitch = 0.0f;
		}
	}
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

bool CameraController::OnWindowResize(WindowResizeEvent& e)
{

	m_Camera.SetProjection(65.0f, (float)e.GetWidth() / (float)e.GetHeight());

	return false;
}

void CameraController::CalculateInputMovement(Timestep ts)
{
	if (Input::IsKeyPressed(EG_KEY_LEFT_SHIFT) && Input::IsMouseButtonPressed(EG_MOUSE_BUTTON_MIDDLE))
	{
		glm::vec3 up = m_Camera.GetVectorUp();
		glm::vec3 right = m_Camera.GetVectorRight();

		m_TargetPosition += up * (m_DeltaMouse.y * ts * m_MovingSensivity);
		m_TargetPosition += right * (m_DeltaMouse.x * ts * m_MovingSensivity * -1.0f);
	}
}
