#include "MainLayer.h"

#include "Engine/Core/Keycodes.h"

#include "Engine/Toolbox/Random.h"

using namespace Engine;

#define rp3dToGlm(vec) glm::vec3(vec.x, vec.y, vec.z)

MainLayer::MainLayer()
	: testTerrain(testNoise, 100)
{
}

void MainLayer::OnAttach()
{
	m_ShaderLibrary.Load("assets/shaders/2D/ScreenQuad.glsl");
	m_ShaderLibrary.Load("assets/shaders/SkinnedModel.glsl");
	m_ShaderLibrary.Load("assets/shaders/Model.glsl");
	m_ShaderLibrary.Load("assets/shaders/SimpleModel.glsl");
	m_ShaderLibrary.Load("assets/shaders/Model_CelShading.glsl");
	m_ShaderLibrary.Load("assets/shaders/DisplayNormals.glsl");
	m_ShaderLibrary.Load("assets/shaders/simpleDepthShader.glsl");
	m_ShaderLibrary.Load("assets/shaders/TerrainShader.glsl");

	model.loadModel("assets/models/well.obj");

	Renderer::InitScene(&m_CameraController.GetCamera(), &m_ShaderLibrary);

	m_FrameBuffer = FrameBuffer::Create({ 1280, 720 });
	m_ScreenQuad = BasicMeshes::ScreenQuad();

	//PointLight light;
	//light.GetLightData().Position = glm::vec4(0.0f, 1.0f, 3.0f, 1.0f);
	//m_PointLights.push_back(light);

	SpotLight light1;
	light1.GetLightData().Position = glm::vec4(0.0f, 1.0f, 3.0f, 1.0f);
	m_SpotLights.push_back(light1);

	testNoise.reseed(12345);

	glDisable(GL_CULL_FACE);
	glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
	glDepthFunc(GL_LEQUAL);
}

void MainLayer::OnDraw(Timestep ts)
{
	//Transform tr; tr.SetPosition(pos);
	//tr.SetPosition(0.0f);
	//Renderer::Submit(model, tr.Get(), "SimpleModel");
	//tr.SetPosition({ 12.0f, 0.0f, 0.0f });
	//Renderer::Submit(model, tr.Get(), "Model_CelShading");


	Renderer::Submit(testTerrain.GetVertexArray(), Material(), glm::mat4(1.0f), false, "TerrainShader");
}

void MainLayer::OnUpdate(Engine::Timestep ts)
{
	m_CameraController.OnUpdate(ts);
	m_Picker.OnUpdate(m_CameraController.GetCamera().GetProjectionMatrix(), m_CameraController.GetCamera().GetViewMatrix());

	EG_TRACE_VEC3(m_Picker.GetCurrentRay());

	Renderer::BeginScene(m_PointLights, m_SpotLights);


	m_FrameBuffer->Bind();
	Engine::RenderCommand::Clear();
	Engine::RenderCommand::SetClearColor({ 135.0f / 255.0f, 206.0f / 255.0f, 250.0f / 255.0f, 1.0f });
	glDisable(GL_STENCIL_TEST);
	/// Render Here:

	//m_Shadows.PreRender(glm::vec3(m_SpotLights[0].GetLightData().Position), {0.0f, 0.0f, 0.0f});
	//OnDraw(ts);
	//m_Shadows.PostRender({m_ShaderLibrary.Get("SimpleModel"), m_ShaderLibrary.Get("Model_CelShading") });
	//
	//m_FrameBuffer->Bind();
	OnDraw(ts);

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
	//m_PointLights[0].OnImGuiRender();
	m_SpotLights[0].OnImGuiRender();
	ImGui::InputDouble("freq", &freq);
	ImGui::InputFloat("spread", &spread);
	ImGui::InputInt("amp", &amp);
	ImGui::InputInt("oct", &octave);
	if (ImGui::Button("ReGenerate!"))
	{
		testTerrain.RegenerateTerrain(freq, octave, amp, spread);
	}
}

void MainLayer::OnEvent(Engine::Event& event)
{
	m_FrameBuffer->OnEvent(event);
	m_CameraController.OnEvent(event);
}

void MainLayer::OnDetach()
{

}