#pragma once

#include "Engine/Networking/Socket.h"
#include "glm/glm.hpp"
#include "Engine/Core/Log.h"
#include <vector>
#include <thread>

#define MAX_PLAYERS 4

namespace Engine
{
	class Client
	{
	public:
		Client(glm::vec4* playerDataStorage);
		~Client();
		void Start(glm::vec4* dataToSend);
		void Close();

	private:
		void RunConnection();

		unsigned int m_MyId = 99;
		glm::vec4 m_PlayersData[MAX_PLAYERS];
		glm::vec4* m_PlayersDataPtr;
		Socket m_Socket;
		std::thread m_Thread;
		SocketData m_ServerData;
		bool m_Closed = true;
		glm::vec4* m_DataToSend = nullptr;
	};

}

