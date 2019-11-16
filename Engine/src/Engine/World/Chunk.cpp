#include "EGpch.h"
#include "Chunk.h"


namespace Engine
{
	Chunk::Chunk()
	{
		m_VertexArray = VertexArray::Create();

		int offset = BLOCK_SIZE;
		for (int x = 0; x < CHUNK_SIZE; x++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				for (int y = 0; y < CHUNK_SIZE; y++)
				{
					glm::vec3 translation(x * offset, y * offset, z * offset);

					int shouldDraw;

					if (x == 0 || x == CHUNK_SIZE - 1 || y == 0 || y == CHUNK_SIZE - 1 || z == 0 || z == CHUNK_SIZE - 1)
						shouldDraw = 1;

					else
						shouldDraw = 1;//shouldDraw = 0;

					m_Translations.push_back(std::make_pair(translation, shouldDraw));

					Block block;
					block.SetPositionInChunk(glm::vec3(x, y, z));
					block.SetType(BlockType::BLOCK_SOLID);
					block.SetIndexInBuffer(m_Translations.size() - 1);
					m_BlocksInChunk[x][y][z] = block;
				}

			}
		}

		for (int x = 0; x < CHUNK_SIZE; x++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				for (int y = 0; y < CHUNK_SIZE; y++)
				{
					Block block = m_BlocksInChunk[x][y][z];

					//Bottom:
					if (y == 0 && x == 0 && z == 0)
					{
						m_Faces.push_back(glm::vec4(1.0f));
						EG_CORE_INFO("PUSH");
					}
					else													  
						m_Faces.push_back(glm::vec4(0.0f));					  
																			  
					//Front:												  
					m_Faces.push_back(glm::vec4(0.0f));
																			  
					//Right:												  
					m_Faces.push_back(glm::vec4(0.0f));
																			  
					//Back:													  
					m_Faces.push_back(glm::vec4(0.0f));
																			  
					//Left:													  
					m_Faces.push_back(glm::vec4(0.0f));
																			  
					//Top:													  
					m_Faces.push_back(glm::vec4(0.0f));
				}

			}
		}

		int x = 0;
		for (int i = 0; i < m_Faces.size(); i++)
		{
			if (m_Faces[i].x == 1) x++;
		}

		EG_CORE_INFO(x);

		float CubesPositions[8 * 3] =
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

		glm::vec3 CubePositions[] = {
			glm::vec3(0.0f,              0.0f,              0.0f),             
			glm::vec3(0.0f + BLOCK_SIZE, 0.0f,              0.0f),            
			glm::vec3(0.0f + BLOCK_SIZE, 0.0f,              0.0f - BLOCK_SIZE), 
			glm::vec3(0.0f,              0.0f,              0.0f - BLOCK_SIZE),

			glm::vec3(0.0f,              0.0f + BLOCK_SIZE, 0.0f),             
			glm::vec3(0.0f + BLOCK_SIZE, 0.0f + BLOCK_SIZE, 0.0f),             
			glm::vec3(0.0f + BLOCK_SIZE, 0.0f + BLOCK_SIZE, 0.0f - BLOCK_SIZE),
			glm::vec3(0.0f,              0.0f + BLOCK_SIZE, 0.0f - BLOCK_SIZE)
		};

		float positions[] = {  //Positions:                        |Normals:            |Tex Coords:                                   
		CubePositions[0].x, CubePositions[0].y, CubePositions[0].z, 0.0f, -1.0f,  0.0f,  0.0f,  0.0f, // BOTTOM 
		CubePositions[1].x, CubePositions[1].y, CubePositions[1].z, 0.0f, -1.0f,  0.0f,  1.0f,  0.0f, // BOTTOM	
		CubePositions[2].x, CubePositions[2].y, CubePositions[2].z, 0.0f, -1.0f,  0.0f,  1.0f,  1.0f, // BOTTOM	
		CubePositions[3].x, CubePositions[3].y, CubePositions[3].z, 0.0f, -1.0f,  0.0f,  0.0f,  1.0f, // BOTTOM	

		CubePositions[0].x, CubePositions[0].y, CubePositions[0].z, 0.0f,  0.0f,  1.0f,  0.0f,  0.0f, //FRONT	
		CubePositions[1].x, CubePositions[1].y, CubePositions[1].z, 0.0f,  0.0f,  1.0f,  1.0f,  0.0f, //FRONT	
		CubePositions[5].x, CubePositions[5].y, CubePositions[5].z, 0.0f,  0.0f,  1.0f,  1.0f,  1.0f, //FRONT	
		CubePositions[4].x, CubePositions[4].y, CubePositions[4].z, 0.0f,  0.0f,  1.0f,  0.0f,  1.0f, //FRONT	

		CubePositions[1].x, CubePositions[1].y, CubePositions[1].z, 1.0f,  0.0f,  0.0f,  0.0f,  0.0f, // RIGHT SIDE
		CubePositions[2].x, CubePositions[2].y, CubePositions[2].z, 1.0f,  0.0f,  0.0f,  1.0f,  0.0f, // RIGHT SIDE
		CubePositions[6].x, CubePositions[6].y, CubePositions[6].z, 1.0f,  0.0f,  0.0f,  1.0f,  1.0f, // RIGHT SIDE
		CubePositions[5].x, CubePositions[5].y, CubePositions[5].z, 1.0f,  0.0f,  0.0f,  0.0f,  1.0f, // RIGHT SIDE

		CubePositions[2].x, CubePositions[2].y, CubePositions[2].z, 0.0f,  0.0f, -1.0f,  0.0f,  0.0f, // BACK		
		CubePositions[3].x, CubePositions[3].y, CubePositions[3].z, 0.0f,  0.0f, -1.0f,  1.0f,  0.0f, // BACK		
		CubePositions[7].x, CubePositions[7].y, CubePositions[7].z, 0.0f,  0.0f, -1.0f,  1.0f,  1.0f, // BACK		
		CubePositions[6].x, CubePositions[6].y, CubePositions[6].z, 0.0f,  0.0f, -1.0f,  0.0f,  1.0f, // BACK		

		CubePositions[3].x, CubePositions[3].y, CubePositions[3].z, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f, // LEFT SIDE
		CubePositions[0].x, CubePositions[0].y, CubePositions[0].z, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f, // LEFT SIDE
		CubePositions[4].x, CubePositions[4].y, CubePositions[4].z, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f, // LEFT SIDE
		CubePositions[7].x, CubePositions[7].y, CubePositions[7].z, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f, // LEFT SIDE

		CubePositions[7].x, CubePositions[7].y, CubePositions[7].z, 0.0f,  1.0f,  0.0f,  0.0f,  0.0f, // TOP       
		CubePositions[4].x, CubePositions[4].y, CubePositions[4].z, 0.0f,  1.0f,  0.0f,  1.0f,  0.0f, // TOP	   
		CubePositions[5].x, CubePositions[5].y, CubePositions[5].z, 0.0f,  1.0f,  0.0f,  1.0f,  1.0f, // TOP		
		CubePositions[6].x, CubePositions[6].y, CubePositions[6].z, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f  // TOP		
		};

		unsigned int CubeIndices[] =
		{
			2, 1, 0, 2, 0, 3, // Bottom
			4, 5, 6, 6, 7, 4, //Front
			8, 9, 10, 10, 11, 8, //Right
			12, 13, 14, 14, 15, 12, //back
			16, 17, 18, 18, 19, 16, //Left
			20, 21, 22, 22, 23, 20 //Top
		};

		Ref<VertexBuffer> ChunkVertexBuffer = VertexBuffer::Create(positions, sizeof(positions));
		Ref<VertexBuffer> PositionsBuffer = VertexBuffer::Create(&m_Translations[0].first.x, sizeof(float) * 4 * m_Translations.size());
		Ref<VertexBuffer> FacesBuffer = VertexBuffer::Create(&m_Faces[0].x, sizeof(float) * 4 * m_Faces.size());
		Ref<IndexBuffer> ChunkIndexBuffer = IndexBuffer::Create(CubeIndices, sizeof(CubeIndices) / sizeof(unsigned int));

		ChunkVertexBuffer->SetLayout(BufferLayout{
			{ShaderDataType::Float3, "a_Positions"},
			{ShaderDataType::Float3, "a_Normals"},
			{ShaderDataType::Float2, "a_TexCoords"}
		});

		PositionsBuffer->SetLayout(BufferLayout{
			{ShaderDataType::Float3, "a_Translations"},
			{ShaderDataType::Float, "a_ShouldDraw"}
		});

		FacesBuffer->SetLayout(BufferLayout{
			{ShaderDataType::Float, "a_Faces"}
		});

		m_VertexArray->AddVertexBuffer(ChunkVertexBuffer);
		m_VertexArray->AddVertexBuffer(PositionsBuffer, true);
		m_VertexArray->AddVertexBuffer(FacesBuffer);
		m_VertexArray->SetIndexBuffer(ChunkIndexBuffer);
	}

	void Chunk::OnImGuiRenderer()
	{
		ImGui::InputInt3("Block coords:", &m_BlockPicker[0]);
		if (ImGui::Button("Change block state"))
		{
			//Ref<VertexBuffer> buffer = m_VertexArray->GetVertexBuffer(1);
			buffer->Bind();

			unsigned int index = m_BlocksInChunk[m_BlockPicker[0]][m_BlockPicker[1]][m_BlockPicker[2]].GetIndex();

			if (m_Translations[index].second == 1)
			{
				float val = 0;
				m_Translations[index].second = val;
				glBufferSubData(GL_ARRAY_BUFFER, (index + 1) * sizeof(float) * 4 - sizeof(float), sizeof(float), &val);
				buffer->Unbind();
				//OnMeshUpdate(index);
			}
			else 
			{
				float val = 1;
				m_Translations[index].second = val;
				glBufferSubData(GL_ARRAY_BUFFER, (index + 1) * sizeof(float) * 4 - sizeof(float), sizeof(float), &val);
				buffer->Unbind();
				//OnMeshUpdate(index);
			}
		}
	}

	void Chunk::OnMeshUpdate(unsigned int index)
	{
		 Ref<VertexBuffer> buffer = m_VertexArray->GetVertexBuffer(1);
		 buffer->Bind();
		//glBufferSubData(GL_ARRAY_BUFFER ,index * sizeof(float) * 4, sizeof(float), )
	}
}