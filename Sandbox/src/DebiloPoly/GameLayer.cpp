#include "EGpch.h"
#include "GameLayer.h"


using namespace Engine;

GameLayer::GameLayer()
	:Layer("GameLayer!"), m_CameraController(65.0f, 1.6f, nullptr), /*testClient(&m_Players[0]), */ world(gravity), shape(1.0f, 2.0f),
	floor_shape(rp3d::Vector3(10.0f, 1.0f, 1.0f))
{
	char yn = 'k'; // change to something else than y or n 
	while (yn != 'y' && yn != 'n')
	{
		EG_INFO("Wanna run a server? (y/n)");
		ZeroMemory(&yn, 1);
		std::cin >> yn;
	}
	

	if (yn == 'y') testServer.Start();

	std::string l_tmp;
	EG_WARN("Please provide server's local ip");
	std::cin >> l_tmp;
	const char* Input_Ip = l_tmp.c_str();
	testClient.SetUp(&m_Players[0], l_tmp.c_str());
	

	EG_CORE_INFO("Game runnin");
	sound.LoadFromFile("assets/sounds/test.wav");

	trShrek.Rotate({ 0.0f, 180.0f, 0.0f });
	testClient.Start(&dataToSend);

}


void GameLayer::OnAttach()
{
	body = world.createRigidBody(transform);
	body->setType(rp3d::BodyType::DYNAMIC);
	proxShape = body->addCollisionShape(&shape, rp3d::Transform::identity(), 80.0f);

	floor = world.createRigidBody(rp3d::Transform::identity());
	floor->setType(rp3d::BodyType::STATIC);
	floor_proxShape = floor->addCollisionShape(&floor_shape, rp3d::Transform::identity(), 4.0f);
	floor->enableGravity(false);

	m_ShaderLibrary.Load("assets/shaders/2D/ScreenQuad.glsl");
	m_ShaderLibrary.Load("assets/shaders/SkinnedModel.glsl");
	m_ShaderLibrary.Load("assets/shaders/DebugDepthQuad.glsl");
	m_ShaderLibrary.Load("assets/shaders/Model.glsl");
	m_ShaderLibrary.Load("assets/game/shaders/TileShader.glsl");


	Renderer::InitScene(&m_CameraController.GetCamera());
	
	m_FrameBuffer = FrameBuffer::Create({ 1280, 720 });
	m_ScreenQuad = BasicMeshes::ScreenQuad();

	testModel.LoadMesh("assets/models/ShrekRunAnim.fbx");
	testModel1.loadModel("assets/game/models/tiles.obj");
	testModel2.loadModel("assets/models/nanosuit/scene.gltf");
	plane.loadModel("assets/models/plane/plane.obj");

	PointLight light;
	light.GetLightData().Position = glm::vec4(0.0f, 1.0f, 3.0f, 1.0f);
	m_PointLights.push_back(light);

	testLevel.Load(&m_ShaderLibrary);

	//clientThread = std::thread(&GameLayer::SendDataToServer, this);
}

void GameLayer::OnUpdate(Engine::Timestep ts)
{
	//rp3d stuff
	const float timeStep = 1.0 / 60.0;
	long double currentFrameTime = Application::GetRunningTime();
	long double deltaTime = currentFrameTime - previousFrameTime;
	previousFrameTime = currentFrameTime;
	accumulator += deltaTime;

	//Updating physics simulation
	while (accumulator >= timeStep)
	{
		world.update(timeStep);

		accumulator -= timeStep;
	}

	rp3d::decimal factor = accumulator / timeStep;
	rp3d::Transform currTransform = body->getTransform();
	rp3d::Transform interpolatedTransform = rp3d::Transform::interpolateTransforms(prevTransform, currTransform, factor);

	Renderer::BeginScene(m_PointLights, m_SpotLights);
	
	m_CameraController.OnUpdate(ts);
	
	m_FrameBuffer->Bind();
	Engine::RenderCommand::Clear();
	Engine::RenderCommand::SetClearColor({ 135.0f / 255.0f, 206.0f / 255.0f, 250.0f / 255.0f, 1.0f });
	
	glDisable(GL_STENCIL_TEST);

	
	//EG_TRACE(angle);
	//Render here:
	trShrek.SetScale(glm::vec3(0.009f));

	dataToSend = glm::vec4(m_CameraController.GetCamera().GetPosition().x, m_CameraController.GetCamera().GetPosition().y, m_CameraController.GetCamera().GetPosition().z, m_CameraController.GetRotation().x);

	// Drawing multiplayer characters!
	for (int i = 0; i < 2; i++)
	{
		trShrek.GetPosition() = glm::vec3(m_Players[i]);
		trShrek.GetRotation().y = m_Players[i].w;
		testModel.OnRender(m_ShaderLibrary.Get("SkinnedModel"), trShrek.Get());
	}

	//Renderer::Submit(plane, Material(), Transform().Get(), false, "Model");

	// Transform trSuit;
	// //EG_TRACE_VEC3(testModel2.GetCenterOfGeometry());
	// trSuit.SetCenterOfGeometry(testModel2.GetCenterOfGeometry());
	// trSuit.SetScale(glm::vec3(0.2f));
	// Renderer::Submit(testModel2, Material(), trSuit.Get(), true, "Model");

	if (Input::IsKeyPressed(EG_KEY_F))
	{
		rp3d::Vector3 force(0.0, 20.0, 0.0);

		// Apply a force to the center of the body 
		//body->applyForceToCenterOfMass(force);
		body->setLinearVelocity(force);
	}

	//rp3d::Vector3 bodyPos = interpolatedTransform.getPosition();
	//trShrek.SetPosition({ bodyPos.x, bodyPos.y, bodyPos.z });
	//testModel.OnRender(m_ShaderLibrary.Get("SkinnedModel"), trShrek.Get());

	testLevel.OnUpdate(ts);

	m_FrameBuffer->Unbind();
	Engine::RenderCommand::Clear();
	Renderer::EndScene(m_FrameBuffer);
	
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);

	m_ShaderLibrary.Get("ScreenQuad")->Bind();
	m_ShaderLibrary.Get("ScreenQuad")->SetInt1("tex", 0);
	m_FrameBuffer->BindTexture(0);
	m_ScreenQuad->Bind();
	RenderCommand::DrawIndexed(m_ScreenQuad);

	prevTransform = currTransform;
}

void GameLayer::OnImGuiRender()
{
	//m_PointLights[0].OnImGuiRender();
	ImGui::Text( "Controls:\n WASD to move around, \n SPACE to go upwards, \n SHIFT to go downwards, \n hold LEFT ALT to control camera");
}

void GameLayer::OnDetach()
{

	testServer.Close();
	testClient.Close();

	Socket::CleanUp();
}

void GameLayer::OnEvent(Engine::Event& event)
{
	m_FrameBuffer->OnEvent(event);
	m_CameraController.OnEvent(event);
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<KeyPressedEvent>(EG_BIND_EVENT_FN(GameLayer::OnKey));
}

