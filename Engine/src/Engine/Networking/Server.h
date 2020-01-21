#pragma once

#include "Engine/Networking/Socket.h"
//#include "Engine/Networking/Client.h"

namespace Engine
{
	class Server
	{
	public:
		Server();

		void ListenForData();

	private:
		SocketData m_ServerHint;
		Socket m_Socket;
		char buffer[1024];
	};
}