#include "EGpch.h"
#include "Client.h"

namespace Engine
{
	Client::Client()
		:m_ServerData("192.168.100.15", 56000)
	{
	}

	bool Client::SendDataToServer(char* dataToSend, unsigned int sizeOfData)
	{
		return m_Socket.SendTo(m_ServerData, dataToSend, sizeOfData);
	}
}


