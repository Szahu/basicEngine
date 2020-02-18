#pragma once


namespace Engine
{
	class Timestep
	{
	public :
		Timestep(float time = 0.0f)
			: m_Time(time)
		{

		}

		operator float() const { return m_Time; }

		float GetSeconds() const { return m_Time; }
		float GetMilliseconds() const { return m_Time * 1000.0f; }

	private:
		float m_Time;
	};

	class Timer
	{
	public:
		Timer(double time = 0.0) { m_Duration = time; }

		void ReStart();
		void SetDuration(double time) { m_Duration = time; } /* @brief in seconds */
		double GetDuration() const { return m_Duration; }
		double GetTimeRemeining() const { return m_TimeRemaining; }

		bool IsFinished();



	private:
		double m_TimeRemaining = 0.0;
		double m_TimePassed = 0.0;
		double m_Duration = 0.0;
		bool m_IsPaused = false;

		double m_StartTime = 0.0;
	};

}