#include "EGpch.h"
#include "Sandbox3D.h"

#include "Glad/glad.h"
#include "imgui.h"


Sandbox3D::Sandbox3D()
	:Layer("Sandbox3D"), m_CameraController(65.0f, 1280.0f / 720.0f)
{

}

void Sandbox3D::OnAttach()
{
	EG_INFO("Sanbox3D online");

	float CubePositions[8 * 3] =
	{  //Positions         | Normals
		-1.0f, -1.0f,  1.0f, 
		 1.0f, -1.0f,  1.0f, 
		 1.0f, -1.0f, -1.0f, 
		-1.0f, -1.0f, -1.0f,
						   
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f
	}; 

	float positions[] = {  //Positions:                                            |Normals:           |Tex Coords:  
		CubePositions[0 * 3], CubePositions[(0 * 3) + 1], CubePositions[(0 * 3) + 2], 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // BOTTOM 
		CubePositions[1 * 3], CubePositions[(1 * 3) + 1], CubePositions[(1 * 3) + 2], 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,// BOTTOM	
		CubePositions[2 * 3], CubePositions[(2 * 3) + 1], CubePositions[(2 * 3) + 2], 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,// BOTTOM	
		CubePositions[3 * 3], CubePositions[(3 * 3) + 1], CubePositions[(3 * 3) + 2], 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,// BOTTOM	
							 			   				 			
		CubePositions[0 * 3], CubePositions[(0 * 3) + 1], CubePositions[(0 * 3) + 2], 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, //FRONT	
		CubePositions[1 * 3], CubePositions[(1 * 3) + 1], CubePositions[(1 * 3) + 2], 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, //FRONT	
		CubePositions[5 * 3], CubePositions[(5 * 3) + 1], CubePositions[(5 * 3) + 2], 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, //FRONT	
		CubePositions[4 * 3], CubePositions[(4 * 3) + 1], CubePositions[(4 * 3) + 2], 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, //FRONT	
							 			   				 			
		CubePositions[1 * 3], CubePositions[(1 * 3) + 1], CubePositions[(1 * 3) + 2], 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // RIGHT SIDE
		CubePositions[2 * 3], CubePositions[(2 * 3) + 1], CubePositions[(2 * 3) + 2], 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // RIGHT SIDE
		CubePositions[6 * 3], CubePositions[(6 * 3) + 1], CubePositions[(6 * 3) + 2], 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // RIGHT SIDE
		CubePositions[5 * 3], CubePositions[(5 * 3) + 1], CubePositions[(5 * 3) + 2], 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // RIGHT SIDE
							 			  				 			
		CubePositions[2 * 3], CubePositions[(2 * 3) + 1], CubePositions[(2 * 3) + 2], 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // BACK		
		CubePositions[3 * 3], CubePositions[(3 * 3) + 1], CubePositions[(3 * 3) + 2], 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // BACK		
		CubePositions[7 * 3], CubePositions[(7 * 3) + 1], CubePositions[(7 * 3) + 2], 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // BACK		
		CubePositions[6 * 3], CubePositions[(6 * 3) + 1], CubePositions[(6 * 3) + 2], 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, // BACK		
							 							 			
		CubePositions[3 * 3], CubePositions[(3 * 3) + 1], CubePositions[(3 * 3) + 2], -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,// LEFT SIDE
		CubePositions[0 * 3], CubePositions[(0 * 3) + 1], CubePositions[(0 * 3) + 2], -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,// LEFT SIDE
		CubePositions[4 * 3], CubePositions[(4 * 3) + 1], CubePositions[(4 * 3) + 2], -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,// LEFT SIDE
		CubePositions[7 * 3], CubePositions[(7 * 3) + 1], CubePositions[(7 * 3) + 2], -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,// LEFT SIDE
							 							 			
		CubePositions[7 * 3], CubePositions[(7 * 3) + 1], CubePositions[(7 * 3) + 2], 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // TOP       
		CubePositions[4 * 3], CubePositions[(4 * 3) + 1], CubePositions[(4 * 3) + 2], 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // TOP	   
		CubePositions[5 * 3], CubePositions[(5 * 3) + 1], CubePositions[(5 * 3) + 2], 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // TOP		
		CubePositions[6 * 3], CubePositions[(6 * 3) + 1], CubePositions[(6 * 3) + 2], 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // TOP		
	};											

	Engine::Ref<Engine::VertexBuffer> t_VertexBuffer = Engine::VertexBuffer::Create(positions, sizeof(positions));


	unsigned int CubeIndices[] =
	{
		0, 1, 2, 2, 0, 3, // Bottom
		4, 5, 6, 6, 4, 7, //Front
		8, 9, 10, 10, 8, 11, //Right
		12, 13, 14, 14, 12, 15, //back
		16, 17, 18, 18, 16, 19, //Left
		20, 21, 22, 22, 20, 23 //Top
	};
	Engine::Ref<Engine::IndexBuffer> t_IndexBuffer = Engine::IndexBuffer::Create(CubeIndices, sizeof(CubeIndices) / sizeof(unsigned int));


	

	glm::vec3 translations[10000];
	int index = 0;
	int offset = 4;
	for (int x = 0; x < 100; x++)
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

	t_VertexBuffer->SetLayout(layout);
	t_TranslationsBuffer->SetLayout(TranslationsLayout);

	m_VertexArray = Engine::VertexArray::Create();
	m_VertexArray->AddVertexBuffer(t_VertexBuffer);
	m_VertexArray->AddVertexBuffer(t_TranslationsBuffer, true);
	m_VertexArray->SetIndexBuffer(t_IndexBuffer);

	
	m_LampVertexArray = Engine::VertexArray::Create();
	m_LampVertexArray->AddVertexBuffer(t_VertexBuffer);
	m_LampVertexArray->SetIndexBuffer(t_IndexBuffer);

	std::string vertexSrc = R"(
		#version 330 core
			
		layout(location = 0) in vec3 a_Positions;
		layout(location = 1) in vec3 a_Normals;
		layout(location = 2) in vec2 a_TexCoords;
		layout(location = 3) in vec3 a_Translations;

		uniform mat4 u_ViewProjectionMatrix;
		uniform mat4 u_Transform;			

		out vec3 v_FragPos;			
		out vec3 v_Normal;			
		out vec2 v_TexCoord;			

		void main() 
		{
			v_TexCoord = a_TexCoords;
			v_FragPos = vec3(u_Transform * vec4(a_Positions, 1.0));
			v_Normal = mat3(transpose(inverse(u_Transform))) * a_Normals;				

			gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Positions.x + a_Translations.x ,a_Positions.y + a_Translations.y , a_Positions.z + a_Translations.z , 1.0);
			//gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Positions.x + (gl_InstanceID * 2.3),a_Positions.y  , a_Positions.z , 1.0);
		}


		)";

	std::string fragemtnSrc = R"(
		#version 330 core
			
		layout(location = 0) out vec4 color;

		uniform vec4 u_Color;
		uniform vec3 u_LightColor;
		uniform vec3 u_LightPosition;
		uniform vec3 u_CameraPosition;

		uniform sampler2D u_Texture;

		in vec3 v_FragPos;			
		in vec3 v_Normal;	
		in vec2 v_TexCoord;			


		void main() 
		{
			vec4 ObjectColor = texture(u_Texture, v_TexCoord);

			// ambient
			float ambientStrength = 0.05;
			vec3 ambient = ambientStrength * u_LightColor;
			
			// diffuse 
			vec3 norm = normalize(v_Normal);
			vec3 lightDir = normalize(u_LightPosition - v_FragPos);
			float diff = max(dot(norm, lightDir), 0.0);
			vec3 diffuse = diff * u_LightColor;

			// specular
			float specularStrength = 0.5;
			vec3 viewDir = normalize(u_CameraPosition - v_FragPos);
			vec3 reflectDir = reflect(-lightDir, norm);
			float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
			vec3 specular = specularStrength * spec * u_LightColor;
			
			vec3 result = (ambient + diffuse + specular) * vec3(ObjectColor);
			color = vec4(result, 1.0);
		}
	
		)";

	m_Shader = Engine::Shader::Create("testShader", vertexSrc, fragemtnSrc);
	m_Shader->Bind();
	m_Shader->SetFloat4("u_Color", glm::vec4(m_CubeColor, 1.0f));
	m_Shader->SetMat4("u_Transform", glm::mat4(1.0f));

	m_FlatColorShader = Engine::Shader::Create("assets/shaders/FlatColor.glsl");
	m_FlatColorShader->Bind();
	m_FlatColorShader->SetFloat4("u_Color", {1.0f, 1.0f, 1.0f, 1.0f});
	m_FlatColorShader->SetMat4("u_Transform", glm::mat4(1.0f));

	m_DirtTexture = Engine::Texture2D::Create("assets/textures/dirt_tex.jpg");
}

void Sandbox3D::OnDetach()
{

}

void Sandbox3D::OnUpdate(Engine::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	FPS = 1.0f / ts;

	Engine::RenderCommand::Clear();
	Engine::RenderCommand::SetClearColor({ 0.06f, 0.06f, 0.06f, 1.0f });

	Engine::Renderer::BeginScene(m_CameraController.GetCamera());
	
	m_Shader->Bind();
	m_Shader->SetFloat4("u_Color", glm::vec4(m_CubeColor, 1.0f));
	m_Shader->SetFloat3("u_LightColor", { 1.0f, 1.0f, 1.0f });
	m_Shader->SetFloat3("u_LightPosition", m_LampPosition);
	m_Shader->SetFloat3("u_CameraPosition", m_CameraController.GetCamera().GetPosition());
	m_Shader->SetInt1("u_Texture", 0);
	m_DirtTexture->Bind();
	m_VertexArray->Bind();
	Engine::Renderer::Submit(m_VertexArray, m_Shader, glm::mat4(1.0f), renderCount);

	m_FlatColorShader->Bind();
	glm::mat4 lampTransform = glm::translate(glm::mat4(1.0f), m_LampPosition) * glm::scale(glm::mat4(1.0f), { 0.5f, 0.5f, 0.5 });
	Engine::Renderer::Submit(m_LampVertexArray, m_FlatColorShader, lampTransform);

	Engine::Renderer::EndScene();
}

void Sandbox3D::OnImGuiRender()
{
	ImGui::Text("FPS: %i", FPS);
	ImGui::ColorPicker3("Cube color", &m_CubeColor.x);
	ImGui::SliderFloat3("Lamp Position", &m_LampPosition.x, -10.0f, 10.0f);
	ImGui::SliderInt("Render Count", &renderCount, 0, 10000);
}

void Sandbox3D::OnEvent(Engine::Event& event)
{
	m_CameraController.OnEvent(event);
}

