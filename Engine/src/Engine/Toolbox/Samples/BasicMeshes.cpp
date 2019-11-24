#include "EGpch.h"
#include "BasicMeshes.h"

namespace Engine
{
	const Ref<VertexArray> Engine::BasicMeshes::Cube()
	{
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

		t_VertexBuffer->SetLayout(Engine::BufferLayout{
			{ Engine::ShaderDataType::Float3, "a_Positions" },
			{ Engine::ShaderDataType::Float3, "a_Normals" },
			{ Engine::ShaderDataType::Float2, "a_TexCoords" },
			});


		Ref<VertexArray> m_CubeVA;

		m_CubeVA = Engine::VertexArray::Create();
		m_CubeVA->AddVertexBuffer(t_VertexBuffer);
		m_CubeVA->SetIndexBuffer(t_IndexBuffer);
		return m_CubeVA;
	}

	const Ref<VertexArray> BasicMeshes::Quad()
	{
		glm::vec4 quad_vertices[4] = {
			{-0.5f, -0.5f, 0.0f, 0.0f},
			{ 0.5f, -0.5f, 1.0f, 0.0f},
			{ 0.5f,  0.5f, 1.0f, 1.0f},
			{-0.5f,  0.5f, 0.0f, 1.0f}
		};

		unsigned int quad_indices[6] = {
			0, 1, 2, 0, 2, 3
		};

		Ref<IndexBuffer> gui_quad_indices = IndexBuffer::Create(quad_indices, 6);
		Ref<VertexBuffer> gui_quad_vertices = VertexBuffer::Create(&quad_vertices[0].x, sizeof(float) * 4 * 4);
		gui_quad_vertices->SetLayout(BufferLayout{
			{ ShaderDataType::Float2, "a_Positions" },
			{ ShaderDataType::Float2, "a_TexCoords" }
			});

		Ref<VertexArray> GuiQuad = VertexArray::Create();

		GuiQuad = VertexArray::Create();
		GuiQuad->AddVertexBuffer(gui_quad_vertices);
		GuiQuad->SetIndexBuffer(gui_quad_indices);
		return GuiQuad;
	}

	const Ref<VertexArray> BasicMeshes::Quad2D()
	{
		float quad_vertices[] = {
			-1.0f,  0.35f, 0.0f, 0.0f, 0.0f,
			-0.65f, 0.35f, 0.0f, 1.0f, 0.0f,
			-0.65f, 0.95f, 0.0f, 1.0f, 1.0f,
			-1.0f,  0.95f, 0.0f, 0.0f, 1.0f 
		};

		unsigned int quad_indices[6] = {
			0, 1, 2, 0, 2, 3
		};

		Ref<IndexBuffer> gui_quad_indices = IndexBuffer::Create(quad_indices, 6);
		Ref<VertexBuffer> gui_quad_vertices = VertexBuffer::Create(quad_vertices, sizeof(quad_vertices));
		gui_quad_vertices->SetLayout(BufferLayout{
			{ ShaderDataType::Float3, "a_Positions" },
			{ ShaderDataType::Float2, "a_TexCoords" }
		});

		Ref<VertexArray> GuiQuad = VertexArray::Create();

		GuiQuad = VertexArray::Create();
		GuiQuad->AddVertexBuffer(gui_quad_vertices);
		GuiQuad->SetIndexBuffer(gui_quad_indices);
		return GuiQuad;
	}
}


