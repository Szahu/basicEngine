#include "EGpch.h"
#include "GameLayer.h"

using namespace Engine;

GameLayer::GameLayer()
	:Layer("GameLayer!"), m_CameraController(65.0f, 1.6f, nullptr),
	testTile(0, "assets/game/models/tile_placeholder_tex.jpg", "dupako", 1000)
{
	EG_CORE_INFO("Game runnin");
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

}

void GameLayer::OnDetach()
{

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
	
	testTile.GetTransform().GetPosition().y += 0.0001;
	m_ShaderLibrary.Get("TileShader")->Bind();
	m_ShaderLibrary.Get("TileShader")->SetInt1("texture_diffuse1", 0);
	testTile.OnRender(m_ShaderLibrary.Get("TileShader"), testModel1);


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
}
