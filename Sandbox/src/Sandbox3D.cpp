#include "EGpch.h"
#include "Sandbox3D.h"

#include "Glad/glad.h"
#include "imgui.h"

#include "GLFW/glfw3.h"

Sandbox3D::Sandbox3D()
	:Layer("Sandbox3D"), m_CameraController(65.0f, 1280.0f / 720.0f), m_Model("assets/Models/SF_Fighter/SciFi_Fighter.FBX"),
	 m_MousePicker(m_CameraController.GetCamera())
{

}

void Sandbox3D::OnAttach()
{
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


	
	
	glm::vec3 translations[10 * 10 * 10];
	
	int index = 0;
	int offset = 1;
	for (int x = 0; x < 10; x++)
	{
		for (int z = 0; z < 10; z++)
		{
			for (int y = 0; y < 10; y++)
			{
				glm::vec3 translation(x * offset, y * offset, z * offset);
				translations[index++] = translation;
			}
			
		}
	} 
	Engine::Ref<Engine::VertexBuffer> t_TranslationsBuffer = Engine::VertexBuffer::Create(&translations[0].x, sizeof(translations));


	Engine::BufferLayout layout{
		{ Engine::ShaderDataType::Float3, "a_Positions" },
		{ Engine::ShaderDataType::Float3, "a_Normals" },
		{ Engine::ShaderDataType::Float2, "a_TexCoords" },
	};

	Engine::BufferLayout TranslationsLayout{
		{ Engine::ShaderDataType::Float3, "a_Translations" }
	};

	t_VertexBuffer->SetLayout(Engine::BufferLayout {
		{ Engine::ShaderDataType::Float3, "a_Positions" },
		{ Engine::ShaderDataType::Float3, "a_Normals" },
		{ Engine::ShaderDataType::Float2, "a_TexCoords" },
		});
	t_TranslationsBuffer->SetLayout(TranslationsLayout);

	m_VertexArray = Engine::VertexArray::Create();
	m_VertexArray->AddVertexBuffer(t_VertexBuffer);
	m_VertexArray->AddVertexBuffer(t_TranslationsBuffer, true);
	m_VertexArray->SetIndexBuffer(t_IndexBuffer);

	
	m_LampVertexArray = Engine::VertexArray::Create();
	m_LampVertexArray->AddVertexBuffer(t_VertexBuffer);
	m_LampVertexArray->SetIndexBuffer(t_IndexBuffer);

	m_ShaderLibrary.Load("assets/shaders/Material.glsl");
	m_ShaderLibrary.Load("assets/shaders/FlatColor.glsl");

	//m_FlatColorShader->Bind();
	//m_FlatColorShader->SetFloat4("u_Color", {1.0f, 1.0f, 1.0f, 1.0f});
	//m_FlatColorShader->SetMat4("u_Transform", glm::mat4(1.0f));

	m_Lights.push_back(&m_Light);
	m_Lights.push_back(&m_Light1);

	m_Framebuffer = Engine::FrameBuffer::Create();
}

void Sandbox3D::OnDetach()
{

}

void Sandbox3D::OnUpdate(Engine::Timestep ts)
{
	m_CameraController.OnUpdate(ts);
	m_MousePicker.OnUpdate();

	FPS = 1.0f / ts;

	Engine::Renderer::BeginScene(m_Framebuffer, m_CameraController.GetCamera(), m_Lights, &m_ShaderLibrary);
	

	m_ShaderLibrary.Get("FlatColor")->Bind();
	m_ShaderLibrary.Get("FlatColor")->SetFloat4("u_Color", { 1.0f, 1.0f, 1.0f, 1.0f });
	Engine::Renderer::Submit(m_Model, glm::mat4(1.0f));

	glm::mat4 lampTransform = glm::translate(glm::mat4(1.0f), glm::vec3(m_LampPosition.x, m_LampPosition.y + sin(glfwGetTime()) * 5, m_LampPosition.z)) * glm::scale(glm::mat4(1.0f), { 0.5f, 0.5f, 0.5 });
	glm::mat4 lampTransform1 = glm::translate(glm::mat4(1.0f), m_LampPosition1) * glm::scale(glm::mat4(1.0f), { 0.5f, 0.5f, 0.5 });
	m_Light.SetPosition(glm::vec3(lampTransform[3][0], lampTransform[3][1], lampTransform[3][2]));
	m_Light1.SetPosition(glm::vec3(lampTransform1[3][0], lampTransform1[3][1], lampTransform1[3][2]));
	m_Framebuffer->BindTexture();
	Engine::Renderer::Submit(m_LampVertexArray, lampTransform1, m_ShaderLibrary.Get("Material"));
	m_Framebuffer->BindTexture();
	Engine::Renderer::Submit(m_LampVertexArray, lampTransform, m_ShaderLibrary.Get("FlatColor"));

	Engine::Renderer::EndScene();
}

void Sandbox3D::OnImGuiRender()
{
	Engine::Gui::InitDocspace();

	Engine::Gui::ViewportWindow(m_Framebuffer);

	bool show = true;
	ImGui::ShowDemoWindow(&show);

	ImGui::Begin("Debug Window");
	ImGui::Text("FPS: %i", FPS);
	ImGui::ColorEdit3("Cube color", &m_CubeColor.x);
	ImGui::SliderFloat3("Lamp Position", &m_LampPosition.x, -20.0f, 20.0f);
	ImGui::SliderFloat3("Lamp Position 1", &m_LampPosition1.x, -20.0f, 20.0f);
	ImGui::SliderInt("Render Count", &renderCount, 0, pow(Engine::Chunk::GetChunkSize(), 3));
	ImGui::ColorEdit3("Lamp Ambient", &amcol.x); m_Light.SetAmbient(amcol);
	ImGui::ColorEdit3("Lamp Diffuse", &diffcol.x); m_Light.SetDiffuse(diffcol);
	ImGui::ColorEdit3("Lamp Speculkar", &speccol.x); m_Light.SetSpecular(speccol);
	ImGui::SliderFloat2("FrameBuffer size", &frameBufferSize.x, 0, 1500);
	ImGui::End();
}

void Sandbox3D::OnEvent(Engine::Event& event)
{
	m_CameraController.OnEvent(event);
	m_Framebuffer->OnEvent(event);
}

