#pragma once

//#include "Engine/World/Block.h"

#include <unordered_map>

#include "glm/glm.hpp"
#include "glad/glad.h"
#include "imgui.h"

#include "Engine/Renderer/PerspectiveCameraController.h"
#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/VertexArray.h"

#include "Engine/World/Block.h"

#define BLOCK_SIZE 1.0f
#define CHUNK_SIZE 16

namespace Engine
{


	class Chunk
	{
	public:
		Chunk();
		~Chunk() = default;

		//void Draw(PerspectiveCameraController camera, const Ref<Shader>& shader);
		void OnImGuiRenderer();
		void OnMeshUpdate(unsigned int index);

		static const unsigned int& GetChunkSize() { return CHUNK_SIZE; }
		static const float& GetBlockSize() { return BLOCK_SIZE; }

		const Ref<VertexArray>& GetVertexArray() const { return m_VertexArray; }

	private:
		Ref<VertexArray> m_VertexArray;
		std::vector<std::pair<glm::vec3,float>> m_Translations;
		std::vector<glm::vec4> m_Faces;
		Block m_BlocksInChunk[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];

		int m_BlockPicker[3] = { 0, 0, 0 };
	};
}