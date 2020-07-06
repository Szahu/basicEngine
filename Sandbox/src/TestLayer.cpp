#include "TestLayer.h"

#include "GLFW/glfw3.h"
#include "Engine/Core/Keycodes.h"

#include <glm/gtc/type_ptr.hpp>

using namespace Engine;

#define rp3dToGlm(vec) glm::vec3(vec.x, vec.y, vec.z)

TestLayer::TestLayer()
:m_CameraController(65.0f, 1.6f, nullptr), m_World(gravity), m_Camera(65.0f, 1.6f)
{
}

void TestLayer::OnAttach()
{
	m_ShaderLibrary.Load("assets/shaders/2D/ScreenQuad.glsl");
	m_ShaderLibrary.Load("assets/shaders/SkinnedModel.glsl");
	m_ShaderLibrary.Load("assets/shaders/Model.glsl");

	m_Terrain.loadModel("assets/models/mountain/terrain.fbx");
	m_Plane.loadModel("assets/models/plane/plane.obj");

	//Renderer::InitScene(&m_CameraController.GetCamera(), &m_ShaderLibrary);
	Renderer::InitScene(&m_Camera);

	m_FrameBuffer = FrameBuffer::Create({ 1280, 720 });
	m_ScreenQuad = BasicMeshes::ScreenQuad();

	PointLight light;
	light.GetLightData().Position = glm::vec4(0.0f, 1.0f, 3.0f, 1.0f);
	m_PointLights.push_back(light);

	tr_Terrain.SetCenterOfGeometry(m_Terrain.GetCenterOfGeometry());
	tr_Terrain.Rotate({ -90.0f, 0.0f, 0.0f });
	tr_Terrain.Translate({ 0.0f, -20.0f, 0.0f });
	tr_Terrain.Scale(glm::vec3(20.0f));

	m_CameraTransform.Rotate({ 0.0f, 180.0f, 0.0f });
}

void TestLayer::OnDetach()
{
}

void TestLayer::OnUpdate(Engine::Timestep ts)
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
		m_World.update(timeStep);

		accumulator -= timeStep;
	}

	rp3d::decimal factor = accumulator / timeStep;
	//rp3d::Transform interpolatedTransform = rp3d::Transform::interpolateTransforms(prevTransform, currTransform, factor);


	float matrix[16];

	Renderer::BeginScene(m_PointLights, m_SpotLights);



	//m_CameraController.OnUpdate(ts);
	//m_Camera.SetTransform(m_CameraTransform);
	m_Camera.SetAngles(M_PI / 180.0f * m_Angles.x , M_PI / 180.0f * m_Angles.y);


	m_FrameBuffer->Bind();
	Engine::RenderCommand::Clear();
	Engine::RenderCommand::SetClearColor({ 135.0f / 255.0f, 206.0f / 255.0f, 250.0f / 255.0f, 1.0f });
	glDisable(GL_STENCIL_TEST);
	/// Render Here:


	Renderer::Submit(m_Terrain, tr_Terrain.Get());
	
	Renderer::Submit(m_Plane, glm::make_mat4(matrix));

	/// Rendering Ends here!
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
}

void TestLayer::OnImGuiRender()
{
	ImGui::SliderFloat2("Angles: ", &m_Angles.x, 0.0f, 360.0f);
}

void TestLayer::OnEvent(Engine::Event& event)
{
}
