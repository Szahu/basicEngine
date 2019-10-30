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
		Model(string const& path)
		{
			loadModel(path);
		}

		//operator Ref<VertexArray>() const { return m_Time; }

		~Model() = default;

		void Draw(const Engine::Ref<Engine::Shader>& shader);
	private:
		//vector<m_Texture> textures_loaded;
		vector<Ref<Texture2D>> m_LoadedTextures;

		vector<Mesh> meshes;
		string directory;

		void loadModel(string path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		vector<Ref<Texture2D>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);

	};
}

