#include "EGpch.h"
#include "Terrain.h"

#include "Glad/glad.h"

using std::vector;

void GridSquare::CalculateCornerPositions()
{
	m_VertexPositions[0] = glm::vec3(m_X, m_Heights[m_Y * m_RowVertexCount + m_X], m_Y);
	m_VertexPositions[1] = glm::vec3(m_X, m_Heights[(m_Y + 1) * m_RowVertexCount + m_X], m_Y + 1);
	m_VertexPositions[2] = glm::vec3(m_X + 1, m_Heights[m_Y * m_RowVertexCount + m_X + 1], m_Y);
	m_VertexPositions[3] = glm::vec3(m_X + 1, m_Heights[(m_Y + 1) * m_RowVertexCount + m_X + 1], m_Y + 1);
}

void GridSquare::CalculateCornerColors()
{
	m_VertexColors[0] = m_Colors[m_Y * m_RowVertexCount + m_X];
	m_VertexColors[1] = m_Colors[(m_Y + 1) * m_RowVertexCount + m_X];
	m_VertexColors[2] = m_Colors[m_Y * m_RowVertexCount + m_X + 1];
	m_VertexColors[3] = m_Colors[(m_Y + 1) * m_RowVertexCount + m_X + 1];
}


Terrain::Terrain(siv::PerlinNoise& noise, int gridSize)
{
	m_PerlinNoise.reseed(12345);
	m_GridSize = gridSize;
	m_PerlinNoise = noise;
	m_HeightGen.Init(m_GridSize, m_PerlinNoise, m_Frequency, m_Octaves, m_Amplitude);
	GenerateMeshData();
	m_VertexCount = calculateVertexCount(m_GridSize + 1);
}

void Terrain::GenerateMeshData()
{

	//IndexGenerator generator;
	//vector<unsigned int> t_IndexData = generator.GenerateIndices(m_GridSize + 1);

	vector<GridSquare> t_lastRowSquares;
	vector<glm::vec3> t_VertexData;

	t_lastRowSquares.resize(m_GridSize);
	vector<float> heights = m_HeightGen.GenerateHeights();

	vector<glm::vec3> colors = m_ColorGen.GenerateColors(heights, m_Amplitude * 3);

	std::thread loadingIndices(&Terrain::LoadIndices, this);

	for (int y = 0; y < m_GridSize; y++)
	{
		for (int x = 0; x < m_GridSize; x++)
		{
			GridSquare square(x, y, m_GridSize, heights, colors);
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

	loadingIndices.join();

	unsigned int essa = m_IndexData[0];

	Engine::Ref<Engine::VertexBuffer> t_VertexBuffer = Engine::VertexBuffer::Create(&t_VertexData[0].x, t_VertexData.size() * sizeof(glm::vec3));
	Engine::Ref<Engine::IndexBuffer> t_IndexBuffer = Engine::IndexBuffer::Create(&m_IndexData[0], m_IndexData.size());


	t_VertexBuffer->SetLayout(Engine::BufferLayout{
			{ Engine::ShaderDataType::Float3, "a_Positions" },
			{ Engine::ShaderDataType::Float3, "a_Normals" },
			{ Engine::ShaderDataType::Float3, "a_Colors" },
	});

	m_VertexArray = Engine::VertexArray::Create();
	m_VertexArray->AddVertexBuffer(t_VertexBuffer);
	m_VertexArray->SetIndexBuffer(t_IndexBuffer);

	vector<unsigned int>().swap(m_IndexData);

}

void Terrain::LoadIndices()
{
	IndexGenerator generator;
	m_IndexData = generator.GenerateIndices(m_GridSize + 1);
}

void Terrain::RegenerateTerrain(double frequency, int octaves, int amplitude)
{


	if (frequency != 0) { m_Frequency = frequency; }
	if (octaves != 0) { m_Octaves = octaves; }
	if (amplitude != 0) { m_Amplitude = amplitude; }

	m_HeightGen.Init(m_GridSize, m_PerlinNoise, m_Frequency, m_Octaves, m_Amplitude);

	vector<glm::vec3> t_VertexData;

	vector<GridSquare> t_lastRowSquares;

	t_lastRowSquares.resize(m_GridSize);
	vector<float> heights = m_HeightGen.GenerateHeights();

	vector<glm::vec3> colors = m_ColorGen.GenerateColors(heights, m_Amplitude * 3);

	for (int y = 0; y < m_GridSize; y++)
	{
		for (int x = 0; x < m_GridSize; x++)
		{
			GridSquare square(x, y, m_GridSize, heights, colors);
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

	GetVertexArray()->GetVertexBuffers()[0]->Bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, t_VertexData.size() * sizeof(glm::vec3), &t_VertexData[0].x);
	GetVertexArray()->GetVertexBuffers()[0]->Unbind();


}


