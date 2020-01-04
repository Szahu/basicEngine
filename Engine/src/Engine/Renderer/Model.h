#pragma once

#include <vector>
#include <map>
#include <string>
#include <iostream>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Shader.h"
#include "Engine/Core/Core.h"
#include "Mesh.h"

#include "Engine/Core/Math3D.h"

using std::vector;
using std::string;
using std::map;


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


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class SkinnedMesh
	{
	public:
		SkinnedMesh();

		//~SkinnedMesh();

		bool LoadMesh(const string& Filename);

		void OnRender(const Ref<Shader>& shader);

		unsigned int NumBones() const {}

		void BoneTransform(float TimeInSeconds, std::vector<Matrix4f>& Transforms);

	private:
	#define NUM_BONES_PER_VEREX 4

		struct BoneInfo
		{
			Matrix4f BoneOffset;
			Matrix4f FinalTransformation;

			BoneInfo()
			{
				BoneOffset.SetZero();
				FinalTransformation.SetZero();
			}
		};

		struct VertexBoneData
		{
			unsigned int IDs[NUM_BONES_PER_VEREX];
			float Weights[NUM_BONES_PER_VEREX];

			VertexBoneData() { Reset(); }
			void Reset() { ZERO_MEM(IDs); ZERO_MEM(Weights); }	

			void AddBoneData(unsigned int BoneID, float Weight);
		};

		void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
		void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
		void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
		unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
		unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
		unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
		const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const string NodeName);
		void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const Matrix4f& ParentTransform);
		bool InitFromScene(const aiScene* pScene, const string& Filename);
		void InitMesh(unsigned int MeshIndex,
			const aiMesh* paiMesh,
			vector<Vector3f>& Positions,
			vector<Vector3f>& Normals,
			vector<Vector2f>& TexCoords,
			vector<VertexBoneData>& Bones,
			vector<unsigned int>& Indices);
		void LoadBones(unsigned int MeshIndex, const aiMesh* paiMesh, vector<VertexBoneData>& Bones);
		bool InitMaterials(const aiScene* pScene, const string& Filename);
		void Clear();

		#define INVALID_MATERIAL 0xFFFFFFFF

		enum VB_TYPES {
			INDEX_BUFFER,
			POS_VB,
			NORMAL_VB,
			TEXCOORD_VB,
			BONE_VB,
			NUM_VBs
		};

		unsigned int m_VAO;
		unsigned int m_Buffers[NUM_VBs];

		struct MeshEntry
		{
			MeshEntry()
			{
				NumIndices    = 0;
				BaseVertex    = 0;
				BaseIndex     = 0;
				MaterialIndex = 0;
			}

			unsigned int NumIndices;
			unsigned int BaseVertex;
			unsigned int BaseIndex;
			unsigned int MaterialIndex;
		};

		vector<MeshEntry> m_Entries;
		vector<Ref<Texture2D>> m_Textures;

		map<string, unsigned int> m_BoneMapping;
		unsigned int m_NumBones;
		vector<BoneInfo> m_BoneInfo;
		Matrix4f m_GlobalInverseTransform;

		const aiScene* m_pScene;
		Assimp::Importer m_Importer;

		Ref<Texture2D> testTexture;
	};


}

