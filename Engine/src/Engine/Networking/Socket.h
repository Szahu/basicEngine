#pragma once

#include <WS2tcpip.h>
#include <winsock.h>
//#pragma comment(lib, "ws2_32.lib")

namespace Engine
{
	class SocketData
	{
	public:

		SocketData(const char* ip, int port);

		SocketData();

		const sockaddr_in& GetAddress() { return m_Addres; }

	private:

		sockaddr_in m_Addres;

	};

	class Socket
	{
	public:
		Socket(SocketData* dataToBind = NULL);

		~Socket();

		bool Bind(const SocketData& data);

		bool SendTo(SocketData& data, char* input, unsigned int size);

		bool RecvFrom(SocketData& socketData, char* buffer, unsigned int size);

		void Close() { m_IsClosed = true;  closesocket(m_Socket); }

		static void CleanUp() { WSACleanup(); }

	private:
		WSADATA data;
		WORD version;
		SOCKET m_Socket;
		char m_ReceiveBuffer[1024];
		bool m_IsClosed = false;
	};
}

