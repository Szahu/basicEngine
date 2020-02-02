#include "EGpch.h"
#include "Socket.h"

namespace Engine
{
	SocketData::SocketData(const char* ip, int port)
	{

		if (ip == NULL)
		{
			m_Addres.sin_addr.S_un.S_addr = ADDR_ANY;
		}
		else
		{
			inet_pton(AF_INET, ip, &m_Addres.sin_addr);
		}

		m_Addres.sin_family = AF_INET;
		int m_AddresLength = sizeof(m_Addres);
		m_Addres.sin_port = htons(port);
	}

	SocketData::SocketData() { ZeroMemory(&m_Addres, sizeof(m_Addres)); }

	Socket::Socket(SocketData* dataToBind)
	{

		version = MAKEWORD(2, 2);
		int wsOK = WSAStartup(version, &data);
		if (wsOK != 0) EG_CORE_ERROR("Cant start Winsock! {0}", wsOK);

		EG_INFO("Winsock started succesfully!");

		m_Socket = socket(AF_INET, SOCK_DGRAM, 0);

		if (dataToBind)
		{
			if (bind(m_Socket, (sockaddr*)&dataToBind->GetAddress(), sizeof(dataToBind->GetAddress())) == SOCKET_ERROR)
			{
				EG_CORE_ERROR("Canty bind socket! {0}", WSAGetLastError());
			}
		}
	}

	Socket::~Socket()
	{
		closesocket(m_Socket);
	}

	bool Socket::Bind(const SocketData& data)
	{
		if (bind(m_Socket, (sockaddr*)&data, sizeof(data)) == SOCKET_ERROR)
		{
			EG_CORE_ERROR("Canty bind socket! {0}", WSAGetLastError());
			return false;
		}

		return true;
	}

	bool Socket::SendTo(SocketData& data, char* input, unsigned int size)
	{
		int sendok = sendto(m_Socket, input, size, 0, (sockaddr*)&data.GetAddress(), sizeof(data.GetAddress()));

		if (sendok == SOCKET_ERROR)
		{
			EG_CORE_ERROR("Sending failed {0}", WSAGetLastError());
			return false;
		}
		return true;
	}

	bool Socket::RecvFrom(SocketData& socketData, char* buffer, unsigned int size)
	{
		int sizeOfData = sizeof(socketData);
		int bytesIn = recvfrom(m_Socket, buffer, size, 0, (sockaddr*)&socketData, &sizeOfData);
		if (bytesIn == SOCKET_ERROR)
		{
			EG_CORE_ERROR("Error receiveing a message! {0}", WSAGetLastError());
			return false;
		}

		return true;
	}
}