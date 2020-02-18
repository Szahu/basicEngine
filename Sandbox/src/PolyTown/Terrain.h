#pragma once

#include "Engine/Toolbox/PerlinNoise.h"
#include "Engine/Renderer/VertexArray.h"
#include "glm/glm.hpp"

#include "glm/gtx/normal.hpp"

// heights.length = m_GridSize + 1;	\

using std::vector;	

class GridSquare
{
public:
	GridSquare() {}
	GridSquare(int x, int y, int gridSize) //Location of top left corner in gird
	{
		m_X = x; m_Y = y;
		m_LastIndex = gridSize - 1;
		CalculateCornerPositions();

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

	void StoreTopLeftVertex(vector<glm::vec3>& data)
	{
		data.push_back(m_VertexPositions[0]);
		data.push_back(m_NormalLeft);
	}

	void StoreTopRightVertex(vector<glm::vec3>& data)
	{
		data.push_back(m_VertexPositions[2]);
		data.push_back(m_NormalRight);
	}

	void StoreBottomLeftVertex(vector<glm::vec3>& data)
	{
		data.push_back(m_VertexPositions[1]);
		data.push_back(m_NormalLeft);
	}

	void StoreBottomRightVertex(vector<glm::vec3>& data)
	{
		data.push_back(m_VertexPositions[3]);
		data.push_back(m_NormalRight);
	}

private:
	int m_X = 0;
	int m_Y = 0;
	int m_LastIndex = 0;
	glm::vec3 m_NormalLeft = glm::vec3(0.0f);
	glm::vec3 m_NormalRight = glm::vec3(0.0f);
	glm::vec3 m_VertexPositions[4];
	bool rightHanded;
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
	Terrain(int gridSize = 4);

	const Engine::Ref<Engine::VertexArray>& GetVertexArray() {return m_VertexArray;}

private:

	void GenerateMeshData();
	int calculateVertexCount(int vertexLength)
	{
		int bottom2Rows = 2 * vertexLength;
		int remainingRowCount = vertexLength - 2;
		int topCount = remainingRowCount * (vertexLength - 1) * 2;
		return topCount + bottom2Rows;
	}
private:
	int m_VertexCount = 0;
	int m_GridSize = 0;
	Engine::Ref<Engine::VertexArray> m_VertexArray;
};