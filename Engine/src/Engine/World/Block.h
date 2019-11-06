#pragma once


#include "glm/glm.hpp"

namespace Engine
{
	enum class BlockType
	{
		None = 0, BLOCK_AIR, BLOCK_SOLID
	};

	class Block
	{
	public:
		void SetType(BlockType type) { m_BlockType = type; }
		void SetPositionInChunk(glm::vec3 pos) { m_PositionInChunk = pos; }
		void SetIndexInBuffer(unsigned int index) { m_IndexInPositionsBuffer = index; }

		const BlockType& GetBlockType() { return m_BlockType; }
		const glm::vec3& GetPositionInChunk() { return m_PositionInChunk; }
		const unsigned int& GetIndex() { return m_IndexInPositionsBuffer; }
	private:
		BlockType m_BlockType = BlockType::None;
		glm::vec3 m_PositionInChunk; 
		unsigned int m_IndexInPositionsBuffer = 0;
	};
}