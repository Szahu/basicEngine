#include "EGpch.h"
#include "GameLayer.h"


using namespace Engine;

GameLayer::GameLayer()
	:Layer("GameLayer!"), m_CameraController(65.0f, 1.6f, nullptr), ServerHint(NULL, 56000), testServer(&ServerHint), serverData("192.168.100.15", 56000)
{
	char yn = 'k'; // change to something else than y or n 
	while (yn != 'y' && yn != 'n')
	{
		EG_INFO("Wanna run a server?");
		ZeroMemory(&yn, 1);
		std::cin >> yn;
	}

	if (yn == 'y') m_RunServer = true;
	else m_RunServer = false;

	EG_CORE_INFO("Game runnin");
	sound.LoadFromFile("assets/sounds/test.wav");
	ZeroMemory(&buffer, 1024);

	trShrek.Rotate({ 0.0f, 180.0f, 0.0f });
}

void GameLayer::SendDataToServer()
{
	char buf[64];
	ZeroMemory(&buf, 64);
	buf[0] = '0';
	Client.SendTo(serverData, buf, 1);
	Client.RecvFrom(serverData, &buf[1], 1);
	myId = int(buf[1]);

	//EG_TRACE(myId);

	while (m_ClientRun)
	{
		float time1 = Application::GetRunningTime();

		ZeroMemory(&buf, 64);

		buf[0] = '1';
		buf[1] = char(myId);
		glm::vec3 pos = m_CameraController.GetCamera().GetPosition();
		
		memcpy(&buf[2], &float(pos.x), 12);
	
		Client.SendTo(serverData, buf, sizeof(buf));

		ZeroMemory(&buf, 64);

		Client.RecvFrom(serverData, buf, sizeof(buf));

		int amountOfPositions = buf[0];

		ZeroMemory(&pos, 12);

		memcpy(&pos, &buf[1], 12);

		m_Players[0] = pos;

		//EG_INFO("{0}, {1}, {2}", m_Players[0].x, m_Players[0].y, m_Players[0].z);


		//EG_TRACE(m_Players[0].x);

		float time2 = Application::GetRunningTime();
		float timeElapsed = time2 - time1;

		float waittime = 100.0f - timeElapsed;
		if ((waittime) > 0) std::this_thread::sleep_for(std::chrono::milliseconds(int(waittime)));
	}
}

void GameLayer::ListenOnServer()
{

	while (m_ServerRun)
	{
		const unsigned int Buffer_size = 64;

		SocketData client;
		char buffer[Buffer_size];
		ZeroMemory(&buffer, Buffer_size);
		bool gotMessage = false;
		if(m_ServerRun) gotMessage = testServer.RecvFrom(client, buffer, Buffer_size);
		
		if (gotMessage)
		{
			//glm::vec3 pos;
			//memcpy(&pos, &buffer[0], 12);
			//EG_INFO("{0}, {1}, {2}", pos.x, pos.y, pos.z);

			switch (buffer[0])
			{
				case '0':
				{ //first connection, give client an id. Layout: [0] = 1 byte
					unsigned int id = m_Clients.size();
					ZeroMemory(&buffer, Buffer_size);
					buffer[0] = char(id);

					EG_INFO("New Client connected from ip: {0}", client.GetAddress().sin_addr.S_un);
					m_Clients.push_back(ClientType(id));

					testServer.SendTo(client, buffer, 1);
				}
				break;

				case '1': //Hey heres my id and position, give me other positions. Layout: [1][id]3x4[pos] = 14 bytes
				{
					unsigned int Client_id = (int)buffer[1];
					glm::vec3 Client_Pos;
					memcpy(&Client_Pos, &buffer[2], 12);

					//EG_INFO("{0}, {1}, {2}", Client_Pos.x, Client_Pos.y, Client_Pos.z);

					//glm::vec3 pos;
					//memcpy(&pos, &buffer[2], 12);
					//EG_INFO("{0}, {1}, {2}", pos.x, pos.y, pos.z);

					EG_ASSERT(Client_id <= m_Clients.size();, "Client Id not in clients vector!");

					m_Clients[Client_id].pos = Client_Pos;

					ZeroMemory(&buffer, Buffer_size);

					//Layout to send: [how many positions]NUMBER_OF_POSITIONS * 12[pos]

					buffer[0] = char(m_Clients.size() - 1);

					unsigned int index = 1;

					for (int i = 0; i < m_Clients.size(); i++)
					{
						if (i == Client_id) continue;
						memcpy(&buffer[index], &m_Clients[i].pos, 12);
						index += 12;
					}

					testServer.SendTo(client, buffer, Buffer_size);

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


void GameLayer::OnAttach()
{


	m_ShaderLibrary.Load("assets/shaders/2D/ScreenQuad.glsl");
	m_ShaderLibrary.Load("assets/shaders/SkinnedModel.glsl");
	m_ShaderLibrary.Load("assets/shaders/DebugDepthQuad.glsl");
	m_ShaderLibrary.Load("assets/shaders/Model.glsl");
	m_ShaderLibrary.Load("assets/game/shaders/TileShader.glsl");

	Renderer::InitScene(&m_CameraController.GetCamera(), &m_ShaderLibrary);
	
	m_FrameBuffer = FrameBuffer::Create({ 1280, 720 });
	m_ScreenQuad = BasicMeshes::ScreenQuad();

	testModel.LoadMesh("assets/models/ShrekRunAnim.fbx");
	testModel1.loadModel("assets/game/models/tiles.obj");

	PointLight light;
	light.GetLightData().Position = glm::vec4(0.0f, 1.0f, 3.0f, 1.0f);
	m_PointLights.push_back(light);

	testLevel.Load(&m_ShaderLibrary);

	//serverThread = std::thread(&Server::ListenForData, Server());
	if(m_RunServer) serverThread = std::thread(&GameLayer::ListenOnServer, this);
	clientThread = std::thread(&GameLayer::SendDataToServer, this);
}

void GameLayer::OnDetach()
{
	if (m_ServerRun && m_RunServer)
	{
		m_ServerRun = false;
		testServer.Close();
		serverThread.join();
	}

	if (m_ClientRun)
	{
		m_ClientRun = false;
		Client.Close();
		clientThread.join();
	}

	Socket::CleanUp();
}

void GameLayer::OnUpdate(Engine::Timestep ts)
{

	Renderer::BeginScene(m_PointLights, m_SpotLights);
	
	m_CameraController.OnUpdate(ts);
	
	m_FrameBuffer->Bind();
	Engine::RenderCommand::Clear();
	Engine::RenderCommand::SetClearColor({ 135.0f / 255.0f, 206.0f / 255.0f, 250.0f / 255.0f, 1.0f });
	
	glDisable(GL_STENCIL_TEST);

	
	//EG_TRACE(angle);
	//Render here:
	trShrek.GetScale() = glm::vec3(0.009f);
	trShrek.GetPosition() = m_Players[0];
	trShrek.GetPosition().y -= 1.0f;	
	trShrek.GetRotation().y = m_CameraController.GetRotation().x;

	testModel.OnRender(m_ShaderLibrary.Get("SkinnedModel"), trShrek.Get());
	
	//EG_INFO(acos(0.5f) * 180/M_PI);

	testLevel.OnUpdate(ts);

	m_FrameBuffer->Unbind();
	Engine::RenderCommand::Clear();
	Renderer::EndScene();
	
	m_ShaderLibrary.Get("ScreenQuad")->Bind();
	m_ShaderLibrary.Get("ScreenQuad")->SetInt1("tex", 0);
	m_FrameBuffer->BindTexture(0);
	m_ScreenQuad->Bind();
	RenderCommand::DrawIndexed(m_ScreenQuad);
}

void GameLayer::OnImGuiRender()
{

}

void GameLayer::OnEvent(Engine::Event& event)
{
	m_FrameBuffer->OnEvent(event);
	m_CameraController.OnEvent(event);
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<KeyPressedEvent>(EG_BIND_EVENT_FN(GameLayer::OnKey));
}

