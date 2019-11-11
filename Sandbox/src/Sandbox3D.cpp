#include "EGpch.h"
#include "Sandbox3D.h"

#include "Glad/glad.h"
#include "imgui.h"

#include "GLFW/glfw3.h"

#include "Engine/Entity/Components/MeshComponent.h"
#include "Engine/Entity/Components/TransformComponent.h"

Sandbox3D::Sandbox3D()
	:Layer("Sandbox3D"), m_CameraController(65.0f, 1280.0f / 720.0f, &m_Window), m_Model("assets/Models/SF_Fighter/SciFi_Fighter.FBX"),
	 m_MousePicker(&m_Window), testEntity("TestEntity"), testEntity1("TestEntity1"), testEntity2("TestEntity2")
{
}

void Sandbox3D::OnAttach()
{
	Engine::Application::Get().SetEditorCameraPointer(&m_CameraController.GetCamera());

	EG_INFO("Sanbox3D online");

	float CubePositions[8 * 3] =
	{  //Positions         | Normals
		-0.5f, -0.5f,  0.5f, 
		 0.5f, -0.5f,  0.5f, 
		 0.5f, -0.5f, -0.5f, 
		-0.5f, -0.5f, -0.5f,
				
		-0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	}; 

	float positions[] = {  //Positions:                                            |Normals:               |Tex Coords:  
		CubePositions[0 * 3], CubePositions[(0 * 3) + 1], CubePositions[(0 * 3) + 2],  0.0f, -1.0f,  0.0f,  0.0f,  0.0f, // BOTTOM 
		CubePositions[1 * 3], CubePositions[(1 * 3) + 1], CubePositions[(1 * 3) + 2],  0.0f, -1.0f,  0.0f,  1.0f,  0.0f, // BOTTOM	
		CubePositions[2 * 3], CubePositions[(2 * 3) + 1], CubePositions[(2 * 3) + 2],  0.0f, -1.0f,  0.0f,  1.0f,  1.0f, // BOTTOM	
		CubePositions[3 * 3], CubePositions[(3 * 3) + 1], CubePositions[(3 * 3) + 2],  0.0f, -1.0f,  0.0f,  0.0f,  1.0f, // BOTTOM	
							 			   				 												    	   
		CubePositions[0 * 3], CubePositions[(0 * 3) + 1], CubePositions[(0 * 3) + 2],  0.0f,  0.0f,  1.0f,  0.0f,  0.0f, //FRONT	
		CubePositions[1 * 3], CubePositions[(1 * 3) + 1], CubePositions[(1 * 3) + 2],  0.0f,  0.0f,  1.0f,  1.0f,  0.0f, //FRONT	
		CubePositions[5 * 3], CubePositions[(5 * 3) + 1], CubePositions[(5 * 3) + 2],  0.0f,  0.0f,  1.0f,  1.0f,  1.0f, //FRONT	
		CubePositions[4 * 3], CubePositions[(4 * 3) + 1], CubePositions[(4 * 3) + 2],  0.0f,  0.0f,  1.0f,  0.0f,  1.0f, //FRONT	
							 			   				 												    	   
		CubePositions[1 * 3], CubePositions[(1 * 3) + 1], CubePositions[(1 * 3) + 2],  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, // RIGHT SIDE
		CubePositions[2 * 3], CubePositions[(2 * 3) + 1], CubePositions[(2 * 3) + 2],  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, // RIGHT SIDE
		CubePositions[6 * 3], CubePositions[(6 * 3) + 1], CubePositions[(6 * 3) + 2],  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, // RIGHT SIDE
		CubePositions[5 * 3], CubePositions[(5 * 3) + 1], CubePositions[(5 * 3) + 2],  1.0f,  0.0f,  0.0f,  0.0f,  1.0f, // RIGHT SIDE
							 			  				 												    	   
		CubePositions[2 * 3], CubePositions[(2 * 3) + 1], CubePositions[(2 * 3) + 2],  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, // BACK		
		CubePositions[3 * 3], CubePositions[(3 * 3) + 1], CubePositions[(3 * 3) + 2],  0.0f,  0.0f, -1.0f,  1.0f,  0.0f, // BACK		
		CubePositions[7 * 3], CubePositions[(7 * 3) + 1], CubePositions[(7 * 3) + 2],  0.0f,  0.0f, -1.0f,  1.0f,  1.0f, // BACK		
		CubePositions[6 * 3], CubePositions[(6 * 3) + 1], CubePositions[(6 * 3) + 2],  0.0f,  0.0f, -1.0f,  0.0f,  1.0f, // BACK		
							 							 												    	   
		CubePositions[3 * 3], CubePositions[(3 * 3) + 1], CubePositions[(3 * 3) + 2], -1.0f,  0.0f,  0.0f,  0.0f,  0.0f, // LEFT SIDE
		CubePositions[0 * 3], CubePositions[(0 * 3) + 1], CubePositions[(0 * 3) + 2], -1.0f,  0.0f,  0.0f,  1.0f,  0.0f, // LEFT SIDE
		CubePositions[4 * 3], CubePositions[(4 * 3) + 1], CubePositions[(4 * 3) + 2], -1.0f,  0.0f,  0.0f,  1.0f,  1.0f, // LEFT SIDE
		CubePositions[7 * 3], CubePositions[(7 * 3) + 1], CubePositions[(7 * 3) + 2], -1.0f,  0.0f,  0.0f,  0.0f,  1.0f, // LEFT SIDE
							 							 												    	   
		CubePositions[7 * 3], CubePositions[(7 * 3) + 1], CubePositions[(7 * 3) + 2],  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, // TOP       
		CubePositions[4 * 3], CubePositions[(4 * 3) + 1], CubePositions[(4 * 3) + 2],  0.0f,  1.0f,  0.0f,  1.0f,  0.0f, // TOP	   
		CubePositions[5 * 3], CubePositions[(5 * 3) + 1], CubePositions[(5 * 3) + 2],  0.0f,  1.0f,  0.0f,  1.0f,  1.0f, // TOP		
		CubePositions[6 * 3], CubePositions[(6 * 3) + 1], CubePositions[(6 * 3) + 2],  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, // TOP		
	};											

	Engine::Ref<Engine::VertexBuffer> t_VertexBuffer = Engine::VertexBuffer::Create(positions, sizeof(positions));


	unsigned int CubeIndices[] =
	{
		2, 1, 0, 2, 0, 3, // Bottom
		4, 5, 6, 6, 7, 4, //Front
		8, 9, 10, 10, 11, 8, //Right
		12, 13, 14, 14, 15, 12, //back
		16, 17, 18, 18, 19, 16, //Left
		20, 21, 22, 22, 23, 20 //Top
	};
	Engine::Ref<Engine::IndexBuffer> t_IndexBuffer = Engine::IndexBuffer::Create(CubeIndices, sizeof(CubeIndices) / sizeof(unsigned int));



	Engine::BufferLayout layout{
		{ Engine::ShaderDataType::Float3, "a_Positions" },
		{ Engine::ShaderDataType::Float3, "a_Normals" },
		{ Engine::ShaderDataType::Float2, "a_TexCoords" },
	};

	t_VertexBuffer->SetLayout(Engine::BufferLayout {
		{ Engine::ShaderDataType::Float3, "a_Positions" },
		{ Engine::ShaderDataType::Float3, "a_Normals" },
		{ Engine::ShaderDataType::Float2, "a_TexCoords" },
		});

	
	m_LampVertexArray = Engine::VertexArray::Create();
	m_LampVertexArray->AddVertexBuffer(t_VertexBuffer);
	m_LampVertexArray->SetIndexBuffer(t_IndexBuffer);

	m_ShaderLibrary.Load("assets/shaders/Material.glsl");
	m_ShaderLibrary.Load("assets/shaders/FlatColor.glsl");

	m_Lights.push_back(&m_Light);
	m_Lights.push_back(&m_Light1);

	testEntity.AddComponent(Engine::ComponentType::Mesh);
	testEntity.GetMeshComponent()->SetVertexArray(m_LampVertexArray);

	testEntity1.AddComponent(Engine::ComponentType::Mesh);
	testEntity1.GetMeshComponent()->SetVertexArray(m_LampVertexArray);

	testEntity2.AddComponent(Engine::ComponentType::Mesh);
	testEntity2.GetMeshComponent()->SetVertexArray(m_LampVertexArray);

	testScene.SetSceneData(m_Window.GetFrameBuffer(), &m_CameraController, m_Lights, &m_ShaderLibrary);
	testScene.AddEntity(Engine::Entity("ImNew"));
	//testScene.AddEntity(testEntity);
	//testScene.AddEntity(testEntity1);
	//testScene.AddEntity(testEntity2);
}

void Sandbox3D::OnDetach()
{

}

void Sandbox3D::OnUpdate(Engine::Timestep ts)
{
	

	testScene.OnUpdate(ts);
	FPS = 1.0f / ts;

	glStencilMask(0x00);

	m_MousePicker.OnUpdate(m_CameraController.GetCamera().GetProjectionMatrix(), m_CameraController.GetCamera().GetViewMatrix());

	glm::mat4 lampTransform1 = glm::translate(glm::mat4(1.0f), m_LampPosition1) * glm::scale(glm::mat4(1.0f), { 0.5f, 0.5f, 0.5 });
	m_Light1.SetPosition(glm::vec3(lampTransform1[3][0], lampTransform1[3][1], lampTransform1[3][2]));

	/*
	testEntity.OnUpdate();

	m_CameraController.OnUpdate(ts);
	m_MousePicker.OnUpdate(m_CameraController.GetCamera().GetProjectionMatrix(), m_CameraController.GetCamera().GetViewMatrix());	

	const glm::vec3 test = { 0.0f, 0.0f, 0.0f };
	float distance = 0.0f;
	bool isInterSecting = glm::intersectRaySphere(m_CameraController.GetCamera().GetPosition(), m_MousePicker.GetCurrentRay(), m_LampPosition1, 1.0f, distance);

	Engine::Renderer::BeginScene(m_Window.GetFrameBuffer(), m_CameraController.GetCamera(), m_Lights, &m_ShaderLibrary);
	
	glStencilMask(0x00);

	m_ShaderLibrary.Get("FlatColor")->Bind();
	m_ShaderLibrary.Get("FlatColor")->SetFloat4("u_Color", { 1.0f, 1.0f, 1.0f, 1.0f });
	Engine::Renderer::Submit(m_Model, testEntity.GetTransformComponent()->GetTransform());

	glm::mat4 lampTransform1 = glm::translate(glm::mat4(1.0f), m_LampPosition1) * glm::scale(glm::mat4(1.0f), { 0.5f, 0.5f, 0.5 });
	m_Light1.SetPosition(glm::vec3(lampTransform1[3][0], lampTransform1[3][1], lampTransform1[3][2]));

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);

	Engine::Renderer::Submit(m_LampVertexArray, lampTransform1, m_ShaderLibrary.Get("FlatColor"));

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	//glDisable(GL_DEPTH_TEST);

	m_ShaderLibrary.Get("FlatColor")->SetFloat4("u_Color", { 0.7f, 2.0f, 0.0f, 1.0f });
	Engine::Renderer::Submit(m_LampVertexArray, glm::scale(lampTransform1, glm::vec3(1.2f, 1.2f, 1.2f)), m_ShaderLibrary.Get("FlatColor"));

	glStencilMask(0x00);
	//glEnable(GL_DEPTH_TEST);

	
	m_ShaderLibrary.Get("FlatColor")->SetFloat4("u_Color", { 1.0f, 1.0f, 1.0f, 1.0f });
	glm::mat4 lampTransform = glm::translate(glm::mat4(1.0f), glm::vec3(m_LampPosition.x, m_LampPosition.y + sin(glfwGetTime()) * 5, m_LampPosition.z)) * glm::scale(glm::mat4(1.0f), { 0.5f, 0.5f, 0.5 });
	m_Light.SetPosition(glm::vec3(lampTransform[3][0], lampTransform[3][1], lampTransform[3][2]));
	//Engine::Renderer::Submit(testEntity.GetMeshComponent()->GetVertexArray(), testEntity.GetTransformComponent()->GetTransform(), m_ShaderLibrary.Get("FlatColor"));


	Engine::Renderer::EndScene();
	*/
}

void Sandbox3D::OnImGuiRender()
{
	Engine::Gui::InitDocspace();

	m_Window.OnImGuiRender();

	bool show = true;
	ImGui::ShowDemoWindow(&show);

	ImGui::Begin("Inspector");
	testScene.OnImGuiRender();
	ImGui::Text("FPS: %i", FPS);
	ImGui::DragFloat3("LightPos", &m_LampPosition1.x, 0.3f);
	ImGui::End();
}

void Sandbox3D::OnEvent(Engine::Event& event)
{
	m_CameraController.OnEvent(event);
	testScene.OnEvent(event);
}

