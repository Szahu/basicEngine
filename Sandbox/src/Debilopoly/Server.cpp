#include "Server.h"

Engine::Server::Server()
	:m_ServerHint(NULL, 56000), m_Server(&m_ServerHint)
{

}

Engine::Server::~Server()
{
	if (!m_Server.isClosed())
	{
		m_Server.Close();
	}
}

void Engine::Server::Start()
{
	m_Thread = std::thread(&Server::RunServer, this);
	m_Closed = false;
}

void Engine::Server::Close()
{
	if (!m_Closed)
	{
		m_Closed = true;
		m_Server.Close();
		m_Thread.join();
	}
}

void Engine::Server::RunServer()
{
	while (!m_Closed)
	{
		const unsigned int Buffer_size = 64;

		SocketData client;
		//char buffer[Buffer_size];
		ZeroMemory(&m_Buffer, Buffer_size);
		bool gotMessage = false;
		gotMessage = m_Server.RecvFrom(client, m_Buffer, Buffer_size);

		if (gotMessage)
		{
			//glm::vec3 pos;
			//memcpy(&pos, &buffer[0], 12);
			//EG_INFO("{0}, {1}, {2}", pos.x, pos.y, pos.z);

			switch (m_Buffer[0])
			{
				case '0':
				{ //first connection, give client an id. Layout: [0] = 1 byte
					unsigned int id = m_Clients.size();
					ZeroMemory(&m_Buffer, Buffer_size);
					m_Buffer[0] = char(id);

					m_Clients.push_back(Client_Data(id));

					m_Server.SendTo(client, m_Buffer, 1);
				}
				break;

				case '1': //Hey heres my id and position, give me other positions. Layout: [1][id]3x4[pos] = 14 bytes
				{
					unsigned int Client_id = (int)m_Buffer[1];
					glm::vec3 Client_Pos;
					memcpy(&Client_Pos, &m_Buffer[2], 12);

					//EG_INFO("{0}, {1}, {2}", Client_Pos.x, Client_Pos.y, Client_Pos.z);

					//glm::vec3 pos;
					//memcpy(&pos, &buffer[2], 12);
					//EG_INFO("{0}, {1}, {2}", pos.x, pos.y, pos.z);

					EG_ASSERT(Client_id <= m_Clients.size(); , "Client Id not in clients vector!");

					m_Clients[Client_id].m_Position = Client_Pos;

					ZeroMemory(&m_Buffer, Buffer_size);

					//Layout to send: [how many positions]NUMBER_OF_POSITIONS * 12[pos]

					m_Buffer[0] = char(m_Clients.size() - 1);

					unsigned int index = 1;

					for (int i = 0; i < m_Clients.size(); i++)
					{
						if (i == Client_id) continue;
						memcpy(&m_Buffer[index], &m_Clients[i].m_Position, 12);
						index += 12;
					}

					m_Server.SendTo(client, m_Buffer, Buffer_size);

				}

				break;

			}

			continue;
		}

		else
		{
			EG_ERROR("Failed in receiving a message!");
			continue;
		}

	}
}
