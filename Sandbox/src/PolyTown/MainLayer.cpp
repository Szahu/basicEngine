#include "MainLayer.h"

#include "Engine/Core/Keycodes.h"

#include "Engine/Toolbox/Random.h"

using namespace Engine;

#define rp3dToGlm(vec) glm::vec3(vec.x, vec.y, vec.z)
#define glmTorp3d(vec) rp3d::Vector3(vec.x, vec.y, vec.z)

MainLayer::MainLayer()
	: testTerrain(&world)
{

	m_ShaderLibrary = Renderer::GetShaderLibrary();

}

void MainLayer::OnAttach()
{
	testTerrain.Generate(testNoise, 100);
	//testTerrain.LoadFromFile("testTerrain.terrain");

	m_ShaderLibrary->Load("assets/shaders/2D/ScreenQuad.glsl");
	m_ShaderLibrary->Load("assets/shaders/SkinnedModel.glsl");
	m_ShaderLibrary->Load("assets/shaders/Model.glsl");
	m_ShaderLibrary->Load("assets/shaders/SimpleModel.glsl");
	m_ShaderLibrary->Load("assets/shaders/Model_CelShading.glsl");
	m_ShaderLibrary->Load("assets/shaders/DisplayNormals.glsl");
	m_ShaderLibrary->Load("assets/shaders/simpleDepthShader.glsl");
	m_ShaderLibrary->Load("assets/shaders/TerrainShader.glsl");
	m_ShaderLibrary->Load("assets/shaders/TerrainShadows.glsl");
	m_ShaderLibrary->Load("assets/shaders/PostProcessing/ColorShift.glsl");
	m_ShaderLibrary->Load("assets/shaders/PostProcessing/Blur.glsl");
	m_ShaderLibrary->Load("assets/shaders/LowPolyModel.glsl");
	m_ShaderLibrary->Load("assets/shaders/LowPolyModelShadows.glsl");
	m_ShaderLibrary->Load("assets/shaders/Model/tester.glsl");

	model.loadModel("assets/models/nanosuit/scene.gltf");

	Renderer::InitScene(&m_CameraController.GetCamera());

	m_FrameBuffer = FrameBuffer::Create();
	m_ScreenQuad = BasicMeshes::ScreenQuad();
	m_ColorShiftBuffer = FrameBuffer::Create(m_FrameBuffer->GetTextureSize(), FrameBuffer::ATTACHMENT_TYPE::COLOR);
	m_BlurBuffer = FrameBuffer::Create(m_FrameBuffer->GetTextureSize(), FrameBuffer::ATTACHMENT_TYPE::COLOR);

	SpotLight light1;
	light1.GetLightData().Position = glm::vec4(0.0f, 1.0f, 3.0f, 1.0f);
	m_SpotLights.push_back(light1);

	PointLight light;
	m_PointLights.push_back(light);

	testNoise.reseed(12345);

	testTerrain.AddShape();

	glDisable(GL_CULL_FACE);
	glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);

	collisionCheck = std::thread(&MainLayer::CheckCollision, this);

	//Renderer::AddPostProcessingEffect("Blur");
	m_Shadows.SetSize(60);
	emiterTimer.SetDuration(1.0f / 60.0f);
	emiterTimer.ReStart();

}

void MainLayer::OnDraw(Timestep ts)
{
	Renderer::Submit(testTerrain.GetVertexArray(), glm::mat4(1.0f), "TerrainShader");
	tr.SetPosition({ 0.0f, 4.0f, 0.0f });
	//Renderer::Submit(model, tr.Get(), "Model");
	Renderer::Submit(model, Material(), tr.Get(), false, "tester");

	//m_ParticleSystem.OnUpdate(ts);
	//m_ParticleSystem.OnRender();

	//if (emiterTimer.IsFinished())
	//{
	//	particle.Position = emiterPos;
	//	particle.ColorBegin = { 1.0f, 0.0f, 0.0f, 1.0f };
	//	particle.ColorEnd = { 0.0f, 1.0f, 0.0f, 0.0f };
	//	particle.SizeBegin = particleSize;
	//	particle.SizeEnd = 0.0f;
	//	m_ParticleSystem.Emit(particle);
	//	emiterTimer.ReStart();
	//
	//}

}

void MainLayer::OnUpdate(Engine::Timestep ts)
{

	m_CameraController.OnUpdate(ts);
	m_Picker.OnUpdate(m_CameraController.GetCamera().GetProjectionMatrix(), m_CameraController.GetCamera().GetViewMatrix());

	Renderer::BeginScene(m_PointLights, m_SpotLights);

	//m_Shadows.PreRender(glm::vec3(50.0f, 25.0f, 50.0f) + glm::vec3(m_SpotLights[0].GetLightData().Direction * -12.0f), { 50.0f, 0.0f, 50.0f });
	//OnDraw(ts);
	//m_Shadows.PostRender({ m_ShaderLibrary->Get("TerrainShadows"), m_ShaderLibrary->Get("LowPolyModelShadows") });

	m_FrameBuffer->Bind();
	Engine::RenderCommand::Clear();
	Engine::RenderCommand::SetClearColor({ 135.0f / 255.0f, 206.0f / 255.0f, 250.0f / 255.0f, 1.0f });
	glDisable(GL_STENCIL_TEST);
	/// Render Here:

	
	//Renderer::Submit(testTerrain.GetVertexArray(), Material(), glm::mat4(1.0f), false, "TerrainShadows");
	OnDraw(ts);

	/// Rendering Ends here!
	m_FrameBuffer->Unbind();
	Engine::RenderCommand::Clear();
	Renderer::EndScene(m_FrameBuffer);


	glDisable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);

	//Post processing
	//Renderer::AddPostProcessingStage(&MainLayer::PP1, this, m_ColorShiftBuffer);
	//Renderer::AddPostProcessingStage(&MainLayer::PP2, this, m_BlurBuffer);
	

	m_ShaderLibrary->Get("ScreenQuad")->Bind();
	m_ShaderLibrary->Get("ScreenQuad")->SetInt1("tex", 0);
	Renderer::GetLastUsedFrameBuffer()->BindTexture(0);
	m_ScreenQuad->Bind();
	RenderCommand::DrawIndexed(m_ScreenQuad);


}

void MainLayer::OnImGuiRender()
{
	//ImGui::SliderFloat3("Pos", &emiterPos.x, -1.0f, 10.0f);
	//ImGui::SliderFloat("Size", &particleSize, 0.0f, 100.0f);
	//ImGui::SliderFloat3("Light Direction", &m_SpotLights[0].GetLightData().Direction.x, -10.0f, 10.0f);
	m_PointLights[0].OnImGuiRender();

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
	m_ColorShiftBuffer->OnEvent(event);
	m_BlurBuffer->OnEvent(event);
}

void MainLayer::CheckCollision()
{
	while (runCollisionTest)
	{

		glm::vec3 camPos = m_CameraController.GetCamera().GetPosition();
		rp3d::Vector3 startPoint = camPos;
		rp3d::Vector3 endPoint = camPos + m_Picker.GetCurrentRay() * rey_length;
		rp3d::Ray mouseRay(startPoint, endPoint);
		rp3d::RaycastInfo raycastInfo;


		if (Input::IsMouseButtonPressed(EG_MOUSE_BUTTON_LEFT) && !testTerrain.IsRegenerating())
		{
			if (testTerrain.GetColiisionBody()->raycast(mouseRay, raycastInfo))
			{
				glm::vec3 point = rp3dToGlm(raycastInfo.worldPoint);
				tr.SetPosition(point); tr.SetScale(0.1f);
			}
			
		}
	}
	
}


void MainLayer::OnDetach()
{
	runCollisionTest = false;
	collisionCheck.join();
}