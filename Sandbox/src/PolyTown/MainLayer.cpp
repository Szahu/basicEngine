#include "MainLayer.h"

#include "Engine/Core/Keycodes.h"


using namespace Engine;

#define rp3dToGlm(vec) glm::vec3(vec.x, vec.y, vec.z)

MainLayer::MainLayer()
	: m_Camera(65.0f, 1.6f)
{
}

void MainLayer::OnAttach()
{
	m_ShaderLibrary.Load("assets/shaders/2D/ScreenQuad.glsl");
	m_ShaderLibrary.Load("assets/shaders/SkinnedModel.glsl");
	m_ShaderLibrary.Load("assets/shaders/Model.glsl");

	m_Terrain.loadModel("assets/models/mountain/terrain.fbx");
	model.loadModel("assets/models/plane/plane.obj");

	Renderer::InitScene(&m_CameraController.GetCamera(), &m_ShaderLibrary);
	//Renderer::InitScene(&m_Camera, &m_ShaderLibrary);

	m_FrameBuffer = FrameBuffer::Create({ 1280, 720 });
	m_ScreenQuad = BasicMeshes::ScreenQuad();

	PointLight light;
	light.GetLightData().Position = glm::vec4(0.0f, 1.0f, 3.0f, 1.0f);
	m_PointLights.push_back(light);

	tr_Terrain.SetCenterOfGeometry(m_Terrain.GetCenterOfGeometry());
	tr_Terrain.Rotate({ -90.0f, 0.0f, 0.0f });
	tr_Terrain.Translate({ 0.0f, -20.0f, 0.0f });
	tr_Terrain.Scale(glm::vec3(20.0f));

}

void MainLayer::OnDetach()
{
}

void MainLayer::OnUpdate(Engine::Timestep ts)
{
	m_CameraController.OnUpdate(ts);
	m_CameraController.SetTargetPosition(pos);

	Renderer::BeginScene(m_PointLights, m_SpotLights);

	m_Camera.SetAngles(M_PI / 180.0f * m_Angles.x, M_PI / 180.0f * m_Angles.y);


	m_FrameBuffer->Bind();
	Engine::RenderCommand::Clear();
	Engine::RenderCommand::SetClearColor({ 135.0f / 255.0f, 206.0f / 255.0f, 250.0f / 255.0f, 1.0f });
	glDisable(GL_STENCIL_TEST);
	/// Render Here:


	Renderer::Submit(m_Terrain, tr_Terrain.Get());
	Transform tr; tr.SetPosition(pos);
	Renderer::Submit(model, tr.Get());

	/// Rendering Ends here!
	m_FrameBuffer->Unbind();
	Engine::RenderCommand::Clear();
	Renderer::EndScene();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);

	m_ShaderLibrary.Get("ScreenQuad")->Bind();
	m_ShaderLibrary.Get("ScreenQuad")->SetInt1("tex", 0);
	m_FrameBuffer->BindTexture(0);
	m_ScreenQuad->Bind();
	RenderCommand::DrawIndexed(m_ScreenQuad);
}

void MainLayer::OnImGuiRender()
{
	ImGui::SliderFloat2("Angles: ", &m_Angles.x, 0.0f, 360.0f);
	m_CameraController.OnImGuiRender();
	ImGui::SliderFloat3("Pos: ", &pos.x, -20.0f, 20.0f);
}

void MainLayer::OnEvent(Engine::Event& event)
{
	m_CameraController.OnEvent(event);
}
