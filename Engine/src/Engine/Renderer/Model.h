#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Shader.h"
#include "Engine/Core/Core.h"
#include "Mesh.h"



using std::vector;
using std::string;



namespace Engine
{

	class Model
	{
	public:

		//operator Ref<VertexArray>() const { return m_Time; }

		~Model() = default;

		const Ref<VertexArray>& GetVertexArray() const { return meshes[0].GetVertexArray(); };

		vector<Mesh>& GetMeshes() { return meshes; }

		bool loadModel(string path);

		void Draw(const Engine::Ref<Engine::Shader>& shader);
	private:
		//vector<m_Texture> textures_loaded;
		vector<Ref<Texture2D>> m_LoadedTextures;

		vector<Mesh> meshes;
		string directory;

		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		vector<Ref<Texture2D>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);

	};
}

