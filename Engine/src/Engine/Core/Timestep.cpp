#include "EGpch.h"
#include "Timestep.h"

#include "Engine/Core/Application.h"


using namespace Engine;

void Engine::Timer::ReStart()
{
	m_StartTime = Application::GetRunningTime();
}

bool Engine::Timer::IsFinished()
{
	double currentTime = Application::GetRunningTime();
	m_TimeRemaining = m_StartTime + m_Duration  - currentTime;
	m_TimePassed = m_Duration - m_TimeRemaining;
	if (m_TimeRemaining <= 0.0) { return true; }
	else return false;
}
