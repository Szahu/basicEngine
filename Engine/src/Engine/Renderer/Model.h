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
		~Model() = default;

		const Ref<VertexArray>& GetVertexArray() const { return m_FinalVA; };

		vector<Mesh>& GetMeshes() { return meshes; }

		bool loadModel(string path);

		void Draw(const Engine::Ref<Engine::Shader>& shader);
		
	private:
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		vector<Ref<Texture2D>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName);
		void SetupVertexArray();
		void LoadTextures();
	private:
		vector<Ref<Texture2D>> m_LoadedTextures;
		vector<std::string> m_DiffuseTexturesPaths;
		vector<Mesh> meshes;
		string directory;

		unsigned int m_MaxTextures = 5;
			
		unsigned int Diffs, Specs, Norms = 0;
		unsigned int DiffsUsed = 0;
		unsigned int SpecsUsed = 0;
		unsigned int NormsUsed = 0;

		Ref<VertexArray> m_FinalVA;
	};
}

