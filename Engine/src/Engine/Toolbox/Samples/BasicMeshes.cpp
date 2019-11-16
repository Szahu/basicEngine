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
}


