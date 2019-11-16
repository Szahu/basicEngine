#pragma once

#include <string>
#include <vector>

#include "Vertex.h"
#include "Shader.h"
#include "Engine/Core/Core.h"
#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/VertexArray.h"

#include "glm/glm.hpp"
#include "Engine/Renderer/Texture.h"

#include "Engine/Entity/Components/MaterialComponent.h"

using std::vector;

namespace Engine
{

	//struct Material
	//{
	//	glm::vec3 Diffuse;
	//	glm::vec3 Specular;
	//	glm::vec3 Ambient;
	//	float Shininess;
	//};

	class Mesh
	{
	public:
		/* Mesh Data */
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Ref<Texture2D>> textures;
		vector<std::pair<unsigned int, unsigned int>> diffBinds; // index, slot
		vector<std::pair<unsigned int, unsigned int>> specBinds; // index, slot
		vector<std::pair<unsigned int, unsigned int>> normBinds; // index, slot
		Material m_material;

		Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Ref<Texture2D>> textures);
		Mesh(vector<Vertex> vertices, vector<unsigned int> indices, Material material);

		const Ref<VertexArray>& GetVertexArray() const { return m_VertexArray; };

		//void Draw(const Engine::Ref<Engine::Shader>& shader);
		void ProccessMaterial(const Engine::Ref<Engine::Shader>& shader);
		bool HasTextureOfType(TextureType type);

	private:
		unsigned int VAO, VBO, EBO;
		Ref<VertexArray> m_VertexArray;
		bool HasTextures;
		void setupMesh();
	};
}

