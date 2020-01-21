#pragma once

#include "Engine/Networking/Socket.h"

namespace Engine
{
	class Client
	{
	public:
		Client();

		bool SendDataToServer(char* dataToSend, unsigned int sizeOfData);

	private:
		SocketData m_ServerData;
		Socket m_Socket;
	};
}
