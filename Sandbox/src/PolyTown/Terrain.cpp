#include "EGpch.h"
#include "Terrain.h"

using std::vector;

void GridSquare::CalculateCornerPositions()
{
	m_VertexPositions[0] = glm::vec3(m_X, 0.0f, m_Y);
	m_VertexPositions[1] = glm::vec3(m_X, 0.0f, m_Y + 1);
	m_VertexPositions[2] = glm::vec3(m_X + 1, 0.0f, m_Y);
	m_VertexPositions[3] = glm::vec3(m_X + 1, 0.0f, m_Y + 1);
}


Terrain::Terrain(int gridSize)
{
	m_GridSize = gridSize;
	GenerateMeshData();
	m_VertexCount = calculateVertexCount(m_GridSize + 1);
}

void Terrain::GenerateMeshData()
{
	IndexGenerator generator;
	vector<glm::vec3> t_VertexData;
	int vertexCount = calculateVertexCount(m_GridSize + 1);
	vector<unsigned int> t_IndexData = generator.GenerateIndices(m_GridSize + 1);

	vector<GridSquare> t_lastRowSquares;

	t_lastRowSquares.resize(m_GridSize);

	for (int y = 0; y < m_GridSize; y++)
	{
		for (int x = 0; x < m_GridSize; x++)
		{
			GridSquare square(x, y, m_GridSize);
			square.StoreSquareData(t_VertexData);
			if (y == m_GridSize - 1)
			{
				t_lastRowSquares[x] = square;
			}
		}

	}
	for (int i = 0; i < t_lastRowSquares.size(); i++)
	{
		t_lastRowSquares[i].StoreBottomRowData(t_VertexData);
	}


	Engine::Ref<Engine::VertexBuffer> t_VertexBuffer = Engine::VertexBuffer::Create(&t_VertexData[0].x, t_VertexData.size() * sizeof(glm::vec3));
	Engine::Ref<Engine::IndexBuffer> t_IndexBuffer = Engine::IndexBuffer::Create(&t_IndexData[0], t_IndexData.size());

	t_VertexBuffer->SetLayout(Engine::BufferLayout{
			{ Engine::ShaderDataType::Float3, "a_Positions" },
			{ Engine::ShaderDataType::Float3, "a_Normals" },
	});

	m_VertexArray = Engine::VertexArray::Create();
	m_VertexArray->AddVertexBuffer(t_VertexBuffer);
	m_VertexArray->SetIndexBuffer(t_IndexBuffer);

}


