#include "EGpch.h"
#include "Terrain.h"

#include "Glad/glad.h"

#include "engine/Core/Timestep.h"

#include <fstream>

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


Terrain::Terrain(rp3d::CollisionWorld* world)
{
	m_CollisionWorld = world;
	rp3d::Vector3 initPosition(50.0, 0.0, 50.0);
	rp3d::Quaternion initOrientation = rp3d::Quaternion::identity();
	rp3d::Transform transform(initPosition, initOrientation);
	m_CollisionBody = m_CollisionWorld->createCollisionBody(transform);

	
}

void Terrain::Generate(siv::PerlinNoise& noise, int gridSize)
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
	//vector<float> heights = m_HeightGen.GenerateHeights();
	m_Heights = m_HeightGen.GenerateHeights();

	vector<glm::vec3> colors = m_ColorGen.GenerateColors(m_Heights, m_Amplitude * 3);
	//vector<glm::vec3> colors = m_ColorGen.GenerateColors(heights, m_HeightGen.GetLowestValue(), m_HeightGen.GetHighestValue());

	std::thread loadingIndices(&Terrain::LoadIndices, this);

	for (int y = 0; y < m_GridSize; y++)
	{
		for (int x = 0; x < m_GridSize; x++)
		{
			GridSquare square(x, y, m_GridSize, m_Heights, colors);
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


	m_CollisionShape = new rp3d::HeightFieldShape(m_GridSize + 1, m_GridSize + 1, m_HeightGen.GetLowestValue(),
		m_HeightGen.GetHighestValue(), &m_Heights[0], rp3d::HeightFieldShape::HeightDataType::HEIGHT_FLOAT_TYPE);

	uint32_t vertexDataSize = t_VertexData.size() * sizeof(glm::vec3);
	uint32_t indexDataSize = m_IndexData.size() * sizeof(unsigned int);

	std::ofstream terrainFile;
	terrainFile.open("testTerrain.terrain", std::ios::out | std::ios::binary);
	terrainFile.seekp(0, std::ios::beg);
	terrainFile.write((char*)&vertexDataSize, sizeof(uint32_t));
	terrainFile.seekp(sizeof(uint32_t), std::ios::cur);
	terrainFile.write((char*)&indexDataSize, sizeof(uint32_t));
	terrainFile.seekp(sizeof(uint32_t), std::ios::cur);
	terrainFile.write((char*)&t_VertexData, vertexDataSize);
	terrainFile.seekp(indexDataSize, std::ios::cur);
	terrainFile.write((char*)&m_IndexData, indexDataSize);
	terrainFile.close();

	vector<unsigned int>().swap(m_IndexData); //Clearing index data

}

void Terrain::LoadFromFile(const char* path)
{
	uint32_t vertexDataSize;
	uint32_t indexDataSize;

	vector<glm::vec3> t_VertexData;

	

	std::ifstream terrainFile;
	terrainFile.open("testTerrain.terrain", std::ios::in | std::ios::binary);
	terrainFile.seekg(0, std::ios::beg);
	terrainFile.read((char*)&vertexDataSize, sizeof(uint32_t));
	terrainFile.seekg(sizeof(uint32_t), std::ios::cur);
	terrainFile.read((char*)&indexDataSize, sizeof(uint32_t));

	t_VertexData.resize(vertexDataSize);
	m_IndexData.resize(indexDataSize);

	terrainFile.seekg(sizeof(uint32_t), std::ios::cur);
	terrainFile.seekg(vertexDataSize, std::ios::cur);
	terrainFile.read((char*)&t_VertexData, vertexDataSize);
	terrainFile.seekg(indexDataSize, std::ios::cur);
	terrainFile.read((char*)&m_IndexData, indexDataSize);
	terrainFile.close();

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

	vector<float> fakeHeights; //fakeHeights.resize((m_GridSize + 1) * (m_GridSize + 1));

	for (int i = 0; i < m_GridSize + 2; i++)
	{
		for (int o = 0; o < m_GridSize + 2; o++)
		{
			fakeHeights.push_back(1);
		}
	}

	m_CollisionShape = new rp3d::HeightFieldShape(100 + 1, 100 + 1, m_HeightGen.GetLowestValue(),
		m_HeightGen.GetHighestValue(), &fakeHeights[0], rp3d::HeightFieldShape::HeightDataType::HEIGHT_FLOAT_TYPE);

	AddShape();

}

void Terrain::LoadIndices()
{
	IndexGenerator generator;
	m_IndexData = generator.GenerateIndices(m_GridSize + 1);
}



void Terrain::RegenerateTerrain(double frequency, int octaves, int amplitude, float spread)
{
	m_IsRegenerating = true;

	m_ColorGen.SetSpread(spread);

	if (frequency != 0) { m_Frequency = frequency; }
	if (octaves != 0) { m_Octaves = octaves; }
	if (amplitude != 0) { m_Amplitude = amplitude; }

	m_HeightGen.Init(m_GridSize, m_PerlinNoise, m_Frequency, m_Octaves, m_Amplitude);

	vector<glm::vec3> t_VertexData;

	vector<GridSquare> t_lastRowSquares;

	t_lastRowSquares.resize(m_GridSize);
	//vector<float> heights = m_HeightGen.GenerateHeights();
	m_Heights = m_HeightGen.GenerateHeights();

	vector<glm::vec3> colors = m_ColorGen.GenerateColors(m_Heights, m_Amplitude * 3);
	//vector<glm::vec3> colors = m_ColorGen.GenerateColors(heights, m_HeightGen.GetLowestValue(), m_HeightGen.GetHighestValue());


	for (int y = 0; y < m_GridSize; y++)
	{
		for (int x = 0; x < m_GridSize; x++)
		{
			GridSquare square(x, y, m_GridSize, m_Heights, colors);
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

	//delete m_CollisionShape;
	//delete m_CollisionBody;

	m_CollisionShape = new rp3d::HeightFieldShape(m_GridSize + 1, m_GridSize + 1, m_HeightGen.GetLowestValue(),
		m_HeightGen.GetHighestValue(), &m_Heights[0], rp3d::HeightFieldShape::HeightDataType::HEIGHT_FLOAT_TYPE);

	rp3d::Vector3 initPosition(50.0, 0.0, 50.0);
	rp3d::Quaternion initOrientation = rp3d::Quaternion::identity();
	rp3d::Transform transform(initPosition, initOrientation);
	m_CollisionBody = m_CollisionWorld->createCollisionBody(transform);

	AddShape();


	m_IsRegenerating = false;
}


