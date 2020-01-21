#include "EGpch.h"
#include "Server.h"

namespace Engine
{
	Server::Server()
		: m_ServerHint(NULL, 56000), m_Socket(&m_ServerHint)
	{
		EG_CORE_INFO("Server Online!");
		ZeroMemory(buffer, 1024);
	}

	void Server::ListenForData()
	{
		ZeroMemory(buffer, 1024);
		SocketData client;
		if (m_Socket.RecvFrom(client, buffer, 1024))
		{
			EG_CORE_INFO("Got data from {0} : {1}", client.GetAddress().sin_addr, buffer);
		}
	}

}

