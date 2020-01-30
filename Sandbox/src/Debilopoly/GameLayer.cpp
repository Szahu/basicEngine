#include "EGpch.h"
#include "GameLayer.h"


using namespace Engine;

GameLayer::GameLayer()
	:Layer("GameLayer!"), m_CameraController(65.0f, 1.6f, nullptr), ServerHint(NULL, 56000), testServer(&ServerHint), serverData("192.168.100.15", 56000)
{
	EG_CORE_INFO("Game runnin");
	sound.LoadFromFile("assets/sounds/test.wav");
	ZeroMemory(&buffer, 1024);

	char buf[100];
	buf[0] = 'd';

	float essa = 1212.123f;
	double f = double(essa);
	//std::string f_string = "#d" + std::to_string(f);

	memcpy(&buf[1], &essa, 4);
	//strcpy(buf, f_string.c_str());

	float d;

	memcpy(&d, &buf[1], 4);

	//EG_INFO(buf);
	EG_INFO(buf[0]);
	EG_INFO(d);
}

void SendDataToServer(Socket& socket, SocketData& server, bool* run, const glm::vec3* vec)
{
	while (*run)
	{
		float time1 = Application::GetRunningTime();

		char buf[12];
		ZeroMemory(&buf, 12);

		glm::vec3 pos = *vec;
		
		memcpy(&buf[0], &float(pos.x), 12);
	
		socket.SendTo(server, buf, sizeof(buf));

		float time2 = Application::GetRunningTime();
		float timeElapsed = time2 - time1;

		float waittime = 256.0f - timeElapsed;

		if ((waittime) > 0) std::this_thread::sleep_for(std::chrono::seconds(int(waittime / 1000.0f)));
	}
}

void ListenOnServer(Socket& socket, bool* run)
{

	while (*run)
	{
		float time1 = Application::GetRunningTime();

		SocketData client;
		char buffer[12];
		ZeroMemory(&buffer, 12);
		bool gotMessage = false;
		if(*run) gotMessage = socket.RecvFrom(client, buffer, 12);
		
		if (gotMessage)
		{
			glm::vec3 pos;
			memcpy(&pos, &buffer[0], 12);
			EG_INFO("{0}, {1}, {2}", pos.x, pos.y, pos.z);

			//float time2 = Application::GetRunningTime();
			//float timeElapsed = time2 - time1;
			//
			//float waittime = 256.0f - timeElapsed;
			//
			//if ((waittime) > 0) std::this_thread::sleep_for(std::chrono::seconds(int(waittime / 1000.0f)));

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
	serverThread = std::thread(ListenOnServer, testServer, &m_ServerRun);
	clientThread = std::thread(SendDataToServer, Client, serverData, &m_ClientRun, &m_CameraController.GetCamera().GetPosition());
}

void GameLayer::OnDetach()
{
	if (m_ServerRun)
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

	//Render here:
	glm::mat4 transform = glm::scale(glm::mat4(1.0f), glm::vec3(0.02f));
	testModel.OnRender(m_ShaderLibrary.Get("SkinnedModel"), transform);
	

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

