#pragma once

#include "SFML/Audio.hpp"


namespace Engine
{
	class Sound
	{
	public:
		Sound(const char* filePath = "")
		{
			if (filePath != "")
			{
				m_Buffer.loadFromFile(filePath);
				m_Sound.setBuffer(m_Buffer);
			}
		}

		const void LoadFromFile(const char* filePath)
		{
			m_Buffer.loadFromFile(filePath);
			m_Sound.setBuffer(m_Buffer);
		}


		const void Play() {m_Sound.play(); }
		const void Pause() {m_Sound.pause(); }
		const void Stop() {m_Sound.stop(); }
		const void SetVolume(const float volume)  {m_Volume = volume; m_Sound.setVolume(m_Volume); }
		const float& GetVolume() { return m_Volume; }
		const void SetLoop(bool ifloop) {m_Sound.setLoop(ifloop); }

	private:
		sf::SoundBuffer m_Buffer;
		sf::Sound m_Sound;
		float m_Volume = 100.0f;
	};
}