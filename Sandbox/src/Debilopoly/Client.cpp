#include "Client.h"

#include "Engine/Core/Application.h"
#include "Engine/Core/Log.h"

Engine::Client::Client(glm::vec4* playerDataStorage)
	:m_ServerData("192.168.100.15", 56000), m_PlayersDataPtr(playerDataStorage)
{

}

Engine::Client::~Client()
{
	Close();
}

void Engine::Client::Start(glm::vec4* dataToSend)
{
	m_DataToSend = dataToSend;
	m_Closed = false;
	m_Thread = std::thread(&Client::RunConnection, this);
}

void Engine::Client::Close()
{
	if (!m_Closed)
	{
		m_Closed = true;
		m_Socket.Close();
		m_Thread.join();
	}
}

void Engine::Client::RunConnection()
{
	char buf[64];
	ZeroMemory(&buf, 64);
	buf[0] = '0'; // case '0' first connection
	m_Socket.SendTo(m_ServerData, buf, 1); //First connection
	m_Socket.RecvFrom(m_ServerData, &buf[1], 1);
	m_MyId = int(buf[1]);

	//EG_TRACE(myId);

	while (!m_Closed)
	{
		float time1 = Application::GetRunningTime();

		ZeroMemory(&buf, 64);

		glm::vec4 data = *m_DataToSend;

		buf[0] = '1'; //first byte is case instructions
		buf[1] = char(m_MyId); //second byte is id of the client
		glm::vec3 pos = glm::vec3(data);
		float camRotation = data.w;
		glm::vec4 dataTosend = { pos.x, pos.y, pos.z, camRotation };

		memcpy(&buf[2], &float(dataTosend.x), 16); //16 bytes of data 12 of position and 4 of rotation

		m_Socket.SendTo(m_ServerData, buf, sizeof(buf));

		ZeroMemory(&buf, 64);

		m_Socket.RecvFrom(m_ServerData, buf, sizeof(buf));

		int amountOfPositions = (int)buf[0];
		//EG_TRACE(amountOfPositions);

		//ZeroMemory(&pos, 12);

		for (int i = 0; i < amountOfPositions; i++)
		{
			//glm::vec4 data;
			memcpy(m_PlayersDataPtr + i, &buf[i * 16 + 1], 16);
			//m_PlayersData[i] = data;
		}

		//memcpy(&pos, &buf[1], 12);
		//
		//m_Players[0] = pos;

		float time2 = Application::GetRunningTime();
		float timeElapsed = time2 - time1;

		float waittime = 100.0f - timeElapsed;
		if ((waittime) > 0) std::this_thread::sleep_for(std::chrono::milliseconds(int(waittime)));
	}
}
