#pragma once

#include "Engine/Networking/Socket.h"
#include "glm/glm.hpp"
#include "Engine/Core/Log.h"
#include <vector>
#include <thread>

namespace Engine
{
	class Server
	{
	private:
		struct Client_Data
		{
			Client_Data(unsigned int id) { m_Id = id; }
			unsigned int m_Id = 99;
			glm::vec3 m_Position = glm::vec3(0.0f);
			float m_Rotation = 0.0f;
		};

	public:
		Server();
		~Server();
		void Start();
		void Close();

		const std::vector<Client_Data>& GetClientData() { return m_Clients; }
		
	private:
		void RunServer();

		std::vector<Client_Data> m_Clients;
		
		SocketData m_ServerHint;
		Socket m_Server;
		bool m_Closed = true;
		std::thread m_Thread;

		const unsigned int m_BufferSize = 64;
		char m_Buffer[64];

	};
}
