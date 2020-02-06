#include "EGpch.h"
#include "GameLayer.h"


using namespace Engine;

GameLayer::GameLayer()
	:Layer("GameLayer!"), m_CameraController(65.0f, 1.6f, nullptr), serverData("192.168.100.15", 56000)
{
	char yn = 'k'; // change to something else than y or n 
	while (yn != 'y' && yn != 'n')
	{
		EG_INFO("Wanna run a server?");
		ZeroMemory(&yn, 1);
		std::cin >> yn;
	}


	if (yn == 'y') testServer.Start();

	EG_CORE_INFO("Game runnin");
	sound.LoadFromFile("assets/sounds/test.wav");

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

		float time2 = Application::GetRunningTime();
		float timeElapsed = time2 - time1;

		float waittime = 100.0f - timeElapsed;
		if ((waittime) > 0) std::this_thread::sleep_for(std::chrono::milliseconds(int(waittime)));
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

	clientThread = std::thread(&GameLayer::SendDataToServer, this);
}

void GameLayer::OnDetach()
{

	testServer.Close();

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

