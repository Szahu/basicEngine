#include "EGpch.h"
#include "GameLayer.h"


using namespace Engine;

GameLayer::GameLayer()
	:Layer("GameLayer!"), m_CameraController(65.0f, 1.6f, nullptr),testClient(&m_Players[0])
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

	//clientThread = std::thread(&GameLayer::SendDataToServer, this);
	testClient.Start(&dataToSend);
}

void GameLayer::OnDetach()
{

	testServer.Close();
	testClient.Close();

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

	dataToSend = glm::vec4(m_CameraController.GetCamera().GetPosition().x, m_CameraController.GetCamera().GetPosition().y, m_CameraController.GetCamera().GetPosition().z, m_CameraController.GetRotation().x);

	for (int i = 0; i < 2; i++)
	{
		trShrek.GetPosition() = glm::vec3(m_Players[i]);
		trShrek.GetRotation().y = m_Players[i].w;
		testModel.OnRender(m_ShaderLibrary.Get("SkinnedModel"), trShrek.Get());
	}

	

	
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

