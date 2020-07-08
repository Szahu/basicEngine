#include "DemoMain.h"

DemoMainLayer::DemoMainLayer()
{
	m_ShaderLibrary = Renderer::GetShaderLibrary();
}

DemoMainLayer::~DemoMainLayer()
{
}

void DemoMainLayer::OnAttach()
{

	//Loading Shaders:
	m_ShaderLibrary->Load("assets/shaders/2D/ScreenQuad.glsl");
	m_ShaderLibrary->Load("assets/shaders/Model.glsl");
	m_ShaderLibrary->Load("assets/shaders/PostProcessing/ColorShift.glsl");
	m_ShaderLibrary->Load("assets/shaders/PostProcessing/Blur.glsl");
	m_ShaderLibrary->Load("assets/shaders/simpleDepthShader.glsl");

	//Loading model
	m_DemoModel.loadModel("assets/models/nanosuit/scene.gltf");
	m_DemoCube = BasicMeshes::Cube();

	//Creating rendering objects
	m_FrameBuffer = FrameBuffer::Create();
	m_ScreenQuad = BasicMeshes::ScreenQuad();
	m_ColorShiftBuffer = FrameBuffer::Create(m_FrameBuffer->GetTextureSize(), FrameBuffer::ATTACHMENT_TYPE::COLOR);
	m_BlurBuffer = FrameBuffer::Create(m_FrameBuffer->GetTextureSize(), FrameBuffer::ATTACHMENT_TYPE::COLOR);

	SpotLight light1;
	light1.GetLightData().Position = glm::vec4(0.0f, 1.0f, 3.0f, 1.0f);
	m_SpotLights.push_back(light1);

	PointLight light;
	m_PointLights.push_back(light);

	Renderer::InitScene(&m_CameraController.GetCamera());

}

void DemoMainLayer::OnDetach()
{
}

void DemoMainLayer::OnDraw(Timestep ts)
{
	Renderer::Submit(m_DemoModel, Material(), glm::mat4(1.0f), false, "Model");

	glm::mat4 cubeTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(100.0f, 1.0f, 100.0f));
	Renderer::Submit(m_DemoCube, Material(), cubeTransform, false, "Model");
}

void DemoMainLayer::OnUpdate(Engine::Timestep ts)
{
	m_CameraController.OnUpdate(ts);
	m_Picker.OnUpdate(m_CameraController.GetCamera().GetProjectionMatrix(), m_CameraController.GetCamera().GetViewMatrix());

	Renderer::BeginScene(m_PointLights, m_SpotLights);

	//Rendering Shadows:
	m_ShadowsRenderer.PreRender(m_SpotLights[0].GetLightData().Position, m_SpotLights[0].GetLightData().Position + m_SpotLights[0].GetLightData().Direction);
	OnDraw(ts);
	m_ShadowsRenderer.PostRender({m_ShaderLibrary->Get("Model")});

	m_FrameBuffer->Bind();
	Engine::RenderCommand::Clear();
	Engine::RenderCommand::SetClearColor({ 135.0f / 255.0f, 206.0f / 255.0f, 250.0f / 255.0f, 1.0f });
	glDisable(GL_STENCIL_TEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);
	glClearColor(0.15f, 0.15f, 0.20f, 1.0f);
	/// Render Here:

	OnDraw(ts);

	/// Rendering Ends here!
	m_FrameBuffer->Unbind();
	Engine::RenderCommand::Clear();
	Renderer::EndScene(m_FrameBuffer);


	glDisable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);


	m_ShaderLibrary->Get("ScreenQuad")->Bind();
	m_ShaderLibrary->Get("ScreenQuad")->SetInt1("tex", 0);
	Renderer::GetLastUsedFrameBuffer()->BindTexture(0);
	m_ScreenQuad->Bind();
	RenderCommand::DrawIndexed(m_ScreenQuad);
}

void DemoMainLayer::OnImGuiRender()
{
	ImGui::Text("Camera Controls: \n Hold mouse scroll to rotate, \n Hold shift + mouse scroll to slide, \n scroll mouse wheel to zoom");

	ImGui::Text("LightControls:");
	m_PointLights[0].OnImGuiRender();
	m_SpotLights[0].OnImGuiRender();
}

void DemoMainLayer::OnEvent(Engine::Event& event)
{
	m_FrameBuffer->OnEvent(event);
	m_CameraController.OnEvent(event);
	m_ColorShiftBuffer->OnEvent(event);
	m_BlurBuffer->OnEvent(event);
}
