#pragma once

#include "Engine/Toolbox/PerlinNoise.hpp"
#include "Engine/Renderer/VertexArray.h"
#include "glm/glm.hpp"

#include "glm/gtx/normal.hpp"
#include "glm/gtx/compatibility.hpp"

#include "rp3d/src/reactphysics3d.h"


// heights.length = m_GridSize + 1;	\

using std::vector;	

class GridSquare
{
public:
	GridSquare() {}
	GridSquare(int x, int y, int gridSize, vector<float>& heights, vector<glm::vec3>& colors) //Location of top left corner in gird
	{
		m_Heights = heights;
		m_Colors = colors;
		m_RowVertexCount = sqrt(m_Heights.size());
		m_X = x; m_Y = y;
		m_LastIndex = gridSize - 1;
		CalculateCornerPositions();
		CalculateCornerColors();

		rightHanded = y % 2 != x % 2;
		m_NormalLeft = glm::triangleNormal(m_VertexPositions[0], m_VertexPositions[1], m_VertexPositions[rightHanded ? 3 : 2]);
		m_NormalRight = glm::triangleNormal(m_VertexPositions[2], m_VertexPositions[rightHanded ? 0 : 1], m_VertexPositions[3]);
	} 


	void StoreSquareData(vector<glm::vec3>& buffer)
	{
		StoreTopLeftVertex(buffer);
		if (m_Y != m_LastIndex || m_X == m_LastIndex)
		{
			StoreTopRightVertex(buffer);
		}
	}

	void StoreBottomRowData(vector<glm::vec3>& buffer)
	{
		if (m_X == 0)
		{
			StoreBottomLeftVertex(buffer);
		}
		StoreBottomRightVertex(buffer);
	}

private:
	void CalculateCornerPositions();
	void CalculateCornerColors();

	void StoreTopLeftVertex(vector<glm::vec3>& data)
	{
		data.push_back(m_VertexPositions[0]);
		data.push_back(m_NormalLeft);
		data.push_back(m_VertexColors[0]);
	}

	void StoreTopRightVertex(vector<glm::vec3>& data)
	{
		data.push_back(m_VertexPositions[2]);
		data.push_back(m_NormalRight);
		data.push_back(m_VertexColors[2]);
	}

	void StoreBottomLeftVertex(vector<glm::vec3>& data)
	{
		data.push_back(m_VertexPositions[1]);
		data.push_back(m_NormalLeft);
		data.push_back(m_VertexColors[1]);
	}

	void StoreBottomRightVertex(vector<glm::vec3>& data)
	{
		data.push_back(m_VertexPositions[3]);
		data.push_back(m_NormalRight);
		data.push_back(m_VertexColors[3]);
	}

private:
	int m_X = 0;
	int m_Y = 0;
	int m_LastIndex = 0;
	glm::vec3 m_NormalLeft = glm::vec3(0.0f);
	glm::vec3 m_NormalRight = glm::vec3(0.0f);
	glm::vec3 m_VertexPositions[4];
	glm::vec3 m_VertexColors[4];
	bool rightHanded;
	vector<float> m_Heights;
	vector<glm::vec3> m_Colors;
	int m_RowVertexCount = 0;
};

class HeightsGenerator
{
public:
	HeightsGenerator(int gridSize, siv::PerlinNoise& noise, double freq, int octaves, float amplitude)
	{
		m_GridSize = gridSize;
		m_Octaves = octaves;
		m_Frequency = freq;
		m_Noise = noise;
		m_Amplitude = amplitude;
	}

	HeightsGenerator() {}

	void Init(int gridSize, siv::PerlinNoise& noise, double freq, int octaves, float amplitude)
	{
		m_GridSize = gridSize;
		m_Octaves = octaves;
		m_Frequency = freq;
		m_Noise = noise;
		m_Amplitude = amplitude;
	}

	vector<float> GenerateHeights()
	{
		vector<float> m_GeneratedHeights;
		for (int y = 0; y < m_GridSize + 1; y++)
		{
			for (int x = 0; x < m_GridSize + 1; x++)
			{
				float height = m_Amplitude * m_Noise.octaveNoise0_1(x / m_Frequency, y / m_Frequency, m_Octaves);
				if (x == 0 && y == 0) { m_HighestValue = height; m_LowestValue = height; }
				if (height > m_HighestValue) m_HighestValue = height;
				if (height < m_LowestValue) m_LowestValue = height;
				m_GeneratedHeights.push_back(height);
			}
		}
		return m_GeneratedHeights;
	}

	void SetPerlinNoise(siv::PerlinNoise& noise) { m_Noise = noise; }
	float GetLowestValue() { return m_LowestValue; }
	float GetHighestValue() { return m_HighestValue; }

private:
	int m_GridSize = 0;
	int m_Octaves = 0;
	double m_Frequency = 0.0;
	siv::PerlinNoise m_Noise;
	float m_Amplitude = 0;
	float m_HighestValue = 0;
	float m_LowestValue = 0;
};

class ColorGenerator
{
public:
	ColorGenerator(float spread = 0.45f)
	{
		m_Spread = spread;
		m_HalfSpread = m_Spread / 2.0f;
		part = 1.0f / (countof(m_TerrainColors) - 1);

		m_TerrainColors[0] = glm::vec3(201.0f / 255.0f, 178.0f / 255.0f, 99.0f / 255.0f);
		m_TerrainColors[1] = glm::vec3(135.0f / 255.0f, 184.0f / 255.0f, 82.0f / 255.0f);
		m_TerrainColors[2] = glm::vec3(80.0f / 255.0f, 171.0f / 255.0f, 93.0f / 255.0f);
		m_TerrainColors[3] = glm::vec3(120.0f / 255.0f, 120.0f / 255.0f, 120.0f / 255.0f);
		m_TerrainColors[4] = glm::vec3(200.0f / 255.0f, 200.0f / 255.0f, 210.0f / 255.0f);
	}

	void SetSpread(float spread) { m_Spread = spread; }

	vector<glm::vec3> GenerateColors(vector<float>& heights, float amplitude)
	{
		vector<glm::vec3> colors;
		int count = sqrt(heights.size());
		for (int y = 0; y < count; y++)
		{
			for (int x = 0; x < count; x++)
			{
				float height = heights[y * count + x];
				glm::vec3 color = CalculateColor(height, amplitude);
				colors.push_back(color);
			}
		}

		return colors;
	}

	vector<glm::vec3> GenerateColors(vector<float>& heights, float lowestHeight, float highestHeight)
	{
		vector<glm::vec3> colors;
		int count = sqrt(heights.size());
		for (int y = 0; y < count; y++)
		{
			for (int x = 0; x < count; x++)
			{
				float height = heights[y * count + x];
				colors.push_back(CalculateColor(height, lowestHeight, highestHeight));
			}
		}

		return colors;
	}

private:
	glm::vec3 CalculateColor(float height, float amplitude)
	{
		float value = (height + amplitude) / (amplitude * 2);
		value = std::clamp((value - m_HalfSpread) * (1.0f / m_Spread), 0.0f, 0.9999f);
		//EG_CORE_INFO(value);
		int firstBiome = (int)glm::floor(value / part);
		float blend = (value - (firstBiome * part)) / part;
		return glm::lerp(m_TerrainColors[firstBiome], m_TerrainColors[firstBiome + 1], blend);
	}

	glm::vec3 CalculateColor(float height, float lowestPoint, float highestPoint)
	{
		float range = highestPoint - lowestPoint;
		float slot = range / _countof(m_TerrainColors);
		float relHeight = height - lowestPoint;
		int index = int(relHeight / slot);
		EG_CORE_ASSERT((index) < _countof(m_TerrainColors));
		if (height == highestPoint) index = _countof(m_TerrainColors) - 1;
		return m_TerrainColors[index];
	}

private:

	float m_Spread = 0;
	float m_HalfSpread = 0;
	float part = 0;
	glm::vec3 m_TerrainColors[5];
};

class IndexGenerator
{
public:

	vector<unsigned int> GenerateIndices(int vertexCount)
	{
		m_VertexCount = vertexCount;
		m_RowLength = (vertexCount - 1) * 2;
		//m_RowLength = 8;

		size_t size = (vertexCount - 1) * (vertexCount - 1) * 6;
		//m_Indices.resize(size);

		storeTopSection();
		storeSecondLastLine();
		storeLastLine();

		return m_Indices;
	}

private:
	void storeTopSection()
	{
		for (int row = 0; row < m_VertexCount - 3; row++)
		{
			for (int col = 0; col < m_VertexCount - 1; col++)
			{
				int topLeft = (row * m_RowLength) + (col * 2);
				int topRight = topLeft + 1;
				int bottomLeft = topLeft + m_RowLength;
				int bottomRight = bottomLeft + 1;
				storeQuad(topLeft, topRight, bottomLeft, bottomRight, col % 2 != row % 2);
			}
		}
	}

	void storeSecondLastLine()
	{
		int row = m_VertexCount - 3;
		for (int col = 0; col < m_VertexCount - 1; col++)
		{
			int topLeft = (row * m_RowLength) + (col * 2);
			int topRight = topLeft + 1;
			int bottomLeft = (topLeft + m_RowLength) - col;
			int bottomRight = bottomLeft + 1;
			storeQuad(topLeft, topRight, bottomLeft, bottomRight, col % 2 != row % 2);
		}
	}

	void storeLastLine()
	{
		int row = m_VertexCount - 2;
		for (int col = 0; col < m_VertexCount - 1; col++)
		{
			int topLeft = (row * m_RowLength) + col;
			int topRight = topLeft + 1;
			int bottomLeft = (topLeft + m_VertexCount);
			int bottomRight = bottomLeft + 1;
			storeLastRowQuad(topLeft, topRight, bottomLeft, bottomRight, col % 2 != row % 2);
		}
	}

	void storeQuad(int topLeft, int topRight, int bottomLeft, int bottomRight, bool rightHanded)
	{
		storeLeftTriangle(topLeft, topRight, bottomLeft, bottomRight, rightHanded);
		//m_Indices[m_IndexPointer++] = topRight;
		//m_Indices[m_IndexPointer++] = rightHanded ? topLeft : bottomLeft;
		//m_Indices[m_IndexPointer++] = bottomRight;
		m_Indices.push_back(topRight);
		m_Indices.push_back(rightHanded ? topLeft : bottomLeft);
		m_Indices.push_back(bottomRight);
	}

	void storeLastRowQuad(int topLeft, int topRight, int bottomLeft, int bottomRight, bool rightHanded)
	{
		storeLeftTriangle(topLeft, topRight, bottomLeft, bottomRight,  rightHanded);
		//m_Indices[m_IndexPointer++] = bottomRight;
		//m_Indices[m_IndexPointer++] = topRight;
		//m_Indices[m_IndexPointer++] = rightHanded ? topLeft : bottomLeft;
		m_Indices.push_back(bottomRight);
		m_Indices.push_back(topRight);
		m_Indices.push_back(rightHanded ? topLeft : bottomLeft);
	}

	void storeLeftTriangle(int topLeft, int topRight, int bottomLeft, int bottomRight, bool rightHanded)
	{
		//m_Indices[m_IndexPointer++] = topLeft;
		//m_Indices[m_IndexPointer++] = bottomLeft;
		//m_Indices[m_IndexPointer++] = rightHanded ? bottomRight : topRight;
		m_Indices.push_back(topLeft);
		m_Indices.push_back(bottomLeft);
		m_Indices.push_back(rightHanded ? bottomRight : topRight);
	}

private:
	int m_VertexCount = 0;
	int m_RowLength = 0;
	vector<unsigned int> m_Indices;
	unsigned int m_IndexPointer = 0;
};

class Terrain
{
public:
	Terrain(siv::PerlinNoise& noise, int gridSize = 4);

	const Engine::Ref<Engine::VertexArray>& GetVertexArray() {return m_VertexArray;}

	void RegenerateTerrain(double frequency = 0, int octaves = 0, int amplitude = 0, float spread = 0.45f);
	
	rp3d::HeightFieldShape* GetCollisionShape() const { return m_CollisionShape; }

private:


	void GenerateMeshData();

	void LoadIndices();

	int calculateVertexCount(int vertexLength)
	{
		int bottom2Rows = 2 * vertexLength;
		int remainingRowCount = vertexLength - 2;
		int topCount = remainingRowCount * (vertexLength - 1) * 2;
		return topCount + bottom2Rows;
	}
private:
	siv::PerlinNoise m_PerlinNoise;
	double m_Frequency = 64.0;
	int m_Octaves = 4;
	int m_VertexCount = 0;
	int m_GridSize = 0;
	int m_Amplitude = 30;
	Engine::Ref<Engine::VertexArray> m_VertexArray;
	HeightsGenerator m_HeightGen;
	ColorGenerator m_ColorGen;
	vector<unsigned int> m_IndexData;
	vector<float> m_Heights;
	rp3d::HeightFieldShape* m_CollisionShape;
};