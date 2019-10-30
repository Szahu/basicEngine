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


using std::vector;

namespace Engine
{

	struct Material
	{
		glm::vec3 Diffuse;
		glm::vec3 Specular;
		glm::vec3 Ambient;
		float Shininess;
	};

	struct m_Texture
	{
		unsigned int id;
		std::string type;
		std::string path;
	};

	class Mesh
	{
	public:
		/* Mesh Data */
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Ref<Texture2D>> textures;
		Material m_material;
		bool HasTextures;

		Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Ref<Texture2D>> textures);
		Mesh(vector<Vertex> vertices, vector<unsigned int> indices, Material material);
		void Draw(const Engine::Ref<Engine::Shader>& shader);

	private:
		unsigned int VAO, VBO, EBO;
		Ref<VertexArray> m_VertexArray;

		void setupMesh();
	};
}

