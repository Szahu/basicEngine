#include "EGpch.h"
#include "GameLayer.h"

using namespace Engine;

GameLayer::GameLayer()
	:Layer("GameLayer!"), m_CameraController(65.0f, 1.6f, nullptr)
{
	EG_CORE_INFO("Game runnin");
}

void GameLayer::OnAttach()
{
	m_ShaderLibrary.Load("assets/shaders/2D/ScreenQuad.glsl");
	m_ShaderLibrary.Load("assets/shaders/SkinnedModel.glsl");
	m_ShaderLibrary.Load("assets/shaders/DebugDepthQuad.glsl");
	Renderer::InitScene(&m_CameraController.GetCamera(), &m_ShaderLibrary);
	
	m_FrameBuffer = FrameBuffer::Create({ 1280, 720 });
	m_ScreenQuad = BasicMeshes::ScreenQuad();

	testModel.LoadMesh("assets/models/ShrekRunAnim.fbx");
}

void GameLayer::OnDetach()
{

}

void GameLayer::OnUpdate(Engine::Timestep ts)
{
	Renderer::BeginScene();
	
	m_CameraController.OnUpdate(ts);
	
	m_FrameBuffer->Bind();
	Engine::RenderCommand::Clear();
	Engine::RenderCommand::SetClearColor({ 0.0f, 1.0f, 0.0f, 1.0f });
	
	glDisable(GL_STENCIL_TEST);

	//Render here:
	glm::mat4 transform = glm::scale(glm::mat4(1.0f), glm::vec3(0.02f));
	testModel.OnRender(m_ShaderLibrary.Get("SkinnedModel"), transform);
	

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
