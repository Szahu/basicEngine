#include "EGpch.h"
#include "Model.h"

#include "glad/glad.h"
#include "Engine/Core/Application.h"

namespace Engine
{
	void Model::Draw(const Engine::Ref<Engine::Shader>& shader)
	{
		for (unsigned int i = 0; i < meshes.size(); i++)
		{
			//meshes[i].Draw(shader);
		}
	}

	bool Model::loadModel(string path)
	{
		// read file via ASSIMP
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		// check for errors
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			EG_CORE_ERROR("ERROR::ASSIMP:: {0}",importer.GetErrorString());
			return false;
		}
		// retrieve the directory path of the filepath
		directory = path.substr(0, path.find_last_of('/'));

		// process ASSIMP's root node recursively
		processNode(scene->mRootNode, scene);

		SetupVertexArray();
		return true;
	}

	void Model::processNode(aiNode* node, const aiScene* scene)
	{
		// process each mesh located at the current node
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			// the node object only contains indices to index the actual objects in the scene. 
			// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}

		// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		// data to fill
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Ref<Texture2D>> textures;

		// Walk through each of the mesh's vertices

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
			// positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;
			// normals
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
			// texture coordinates
			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			// tangent
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;
			// bitangent
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;
			vertices.push_back(vertex);
		}
		// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		// process materials
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
		// Same applies to other texture as the following list summarizes:
		// diffuse: texture_diffuseN
		// specular: texture_specularN
		// normal: texture_normalN

		Material m_Material;
		aiColor3D color(0.f, 0.f, 0.f);
		float shininess;

		material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		m_Material.m_Diffuse = glm::vec3(color.r, color.b, color.g);

		material->Get(AI_MATKEY_COLOR_AMBIENT, color);
		m_Material.m_Ambient = glm::vec3(color.r, color.b, color.g);

		material->Get(AI_MATKEY_COLOR_SPECULAR, color);
		m_Material.m_Specular = glm::vec3(color.r, color.b, color.g);

		material->Get(AI_MATKEY_SHININESS, shininess);
		m_Material.m_Shininess = shininess;


		// 1. diffuse maps
		vector<Ref<Texture2D>> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::Diffuse);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. specular maps
		vector<Ref<Texture2D>> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::Specular);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. normal maps
		std::vector<Ref<Texture2D>> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, TextureType::Normal);
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		// 4. height maps
		std::vector<Ref<Texture2D>> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, TextureType::Height);
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		// return a mesh object created from the extracted mesh data
		//EG_CORE_INFO(textures.size());
		if (textures.size() == 0) return Mesh(vertices, indices, m_Material);
		else return Mesh(vertices, indices, textures);
	}

	vector<Ref<Texture2D>> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName)
	{
		vector<Ref<Texture2D>> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			bool skip = false;
			for (unsigned int j = 0; j < m_LoadedTextures.size(); j++)
			{
				if (std::strcmp(m_LoadedTextures[j]->GetPath().c_str(), str.C_Str()) == 0)
				{
					textures.push_back(m_LoadedTextures[j]);
					skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
					break;
				}
			}
			if (!skip)
			{   // if texture hasn't been loaded already, load it

				Ref<Texture2D> tex = Texture2D::Create(this->directory + '/' + str.C_Str());
				tex->SetType(typeName);
				textures.push_back(tex);
			}
		}

		return textures;
	}

	void Model::SetupVertexArray()
	{
		m_FinalVA = VertexArray::Create();

		std::vector<float> vertexBufferData;
		std::vector<unsigned int> indexBufferData;

		unsigned int indexOffset = 0;

		for (int i = 0; i < meshes.size(); i++)
		{
			for (int t = 0; t < meshes[i].vertices.size(); t++)
			{
				vertexBufferData.push_back(meshes[i].vertices[t].Position.x);
				vertexBufferData.push_back(meshes[i].vertices[t].Position.y);
				vertexBufferData.push_back(meshes[i].vertices[t].Position.z);

				vertexBufferData.push_back(meshes[i].vertices[t].Normal.x);
				vertexBufferData.push_back(meshes[i].vertices[t].Normal.y);
				vertexBufferData.push_back(meshes[i].vertices[t].Normal.z);

				vertexBufferData.push_back(meshes[i].vertices[t].TexCoords.x);
				vertexBufferData.push_back(meshes[i].vertices[t].TexCoords.y);

				vertexBufferData.push_back(meshes[i].vertices[t].Tangent.x);
				vertexBufferData.push_back(meshes[i].vertices[t].Tangent.y);
				vertexBufferData.push_back(meshes[i].vertices[t].Tangent.z);

				vertexBufferData.push_back(meshes[i].vertices[t].Bitangent.x);
				vertexBufferData.push_back(meshes[i].vertices[t].Bitangent.y);
				vertexBufferData.push_back(meshes[i].vertices[t].Bitangent.z);
			}

			for (int t = 0; t < meshes[i].indices.size(); t++)
			{
				unsigned int temp = meshes[i].indices[t];
				temp += indexOffset;
				indexBufferData.push_back(temp);
			}

			auto it = std::max_element(std::begin(meshes[i].indices), std::end(meshes[i].indices));
			indexOffset += *it + 1;

		}

		Ref<VertexBuffer> vb = VertexBuffer::Create(&vertexBufferData[0], sizeof(float) * vertexBufferData.size());
		vb->SetLayout(BufferLayout{
			{ShaderDataType::Float3, "a_Positions"},
			{ShaderDataType::Float3, "a_Normals"},
			{ShaderDataType::Float2, "a_TexCoords"},
			{ShaderDataType::Float3, "a_Tangents"},
			{ShaderDataType::Float3, "a_BiTangents"}
		});

		Ref<IndexBuffer> ib = IndexBuffer::Create(&indexBufferData[0], indexBufferData.size());

		m_FinalVA->AddVertexBuffer(vb);
		m_FinalVA->SetIndexBuffer(ib);
	}

	void Model::LoadTextures()
	{

	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	#define POSITION_LOCATION    0
	#define NORMAL_LOCATION      1
	#define TEX_COORD_LOCATION   2
	#define BONE_ID_LOCATION     3
	#define BONE_WEIGHT_LOCATION 4
	#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices)

	SkinnedMesh::SkinnedMesh()
	{
		m_VAO = 0;
		ZERO_MEM(m_Buffers);
		m_NumBones = 0;
		m_pScene = NULL;

		testTexture = Texture2D::Create("assets/Textures/test.png");
	}

	//SkinnedMesh::~SkinnedMesh()
	//{
	//	Clear();
	//}

	bool SkinnedMesh::LoadMesh(const string& Filename)
	{
		// Release the previously loaded mesh (if it exists)
		Clear();

		// Create the VAO
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		// Create the buffers for the vertices attributes
		glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);

		bool Ret = false;

		m_pScene = m_Importer.ReadFile(Filename.c_str(), ASSIMP_LOAD_FLAGS);

		if (m_pScene)
		{
			m_GlobalInverseTransform = m_pScene->mRootNode->mTransformation;
			m_GlobalInverseTransform.Inverse();
			Ret = InitFromScene(m_pScene, Filename);
		}
		else
		{
			EG_CORE_ERROR("Error loading a file: {0} :: {1}", Filename.c_str(), m_Importer.GetErrorString());
			EG_CORE_ASSERT(false, "");
		}

		// Make sure the VAO is not changed from the outside
		glBindVertexArray(0);

		return Ret;
	}

	void SkinnedMesh::OnRender(const Ref<Shader>& shader,const glm::mat4& transform)
	{
		glBindVertexArray(m_VAO);

		shader->Bind();

		BoneTransform(Application::GetRunningTime());
		glUniformMatrix4fv(glGetUniformLocation(shader->GetID(), "gBones"), m_BoneTransforms.size(), GL_TRUE, (const GLfloat*)m_BoneTransforms[0]);

		for (unsigned int i = 0; i < m_Entries.size(); i++)
		{
			const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

			EG_CORE_ASSERT(MaterialIndex < m_Textures.size(), "Material Index out of m_Textures range!");

			shader->SetInt1("texture_diffuse1", 0);
			shader->SetMat4("u_Transform", transform);
			m_Textures[MaterialIndex]->Bind(0);

			glDrawElementsBaseVertex(GL_TRIANGLES,
				m_Entries[i].NumIndices,
				GL_UNSIGNED_INT,
				(void*)(sizeof(unsigned int) * m_Entries[i].BaseIndex),
				m_Entries[i].BaseVertex);

			//glDrawElements(GL_TRIANGLES, m_Entries[i].NumIndices, GL_UNSIGNED_INT, nullptr);
		}

		// Make sure the VAO is not changed from the outside    
		glBindVertexArray(0);
	}

	void SkinnedMesh::BoneTransform(float TimeInSeconds)
	{
		Matrix4f Identity;
		Identity.InitIdentity();

		float TicksPerSecond = (float)(m_pScene->mAnimations[0]->mTicksPerSecond != 0 ? m_pScene->mAnimations[0]->mTicksPerSecond : 25.0f);
		float TimeInTicks = TimeInSeconds * TicksPerSecond;
		float AnimationTime = fmod(TimeInTicks, (float)m_pScene->mAnimations[0]->mDuration);

		ReadNodeHeirarchy(AnimationTime, m_pScene->mRootNode, Identity);

		m_BoneTransforms.resize(m_NumBones);

		for (unsigned int i = 0; i < m_NumBones; i++)
		{
			m_BoneTransforms[i] = m_BoneInfo[i].FinalTransformation;
		}
	}

	void SkinnedMesh::VertexBoneData::AddBoneData(unsigned int BoneID, float Weight)
	{
		for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(IDs); i++)
		{
			if (Weights[i] == 0.0)
			{
				IDs[i] = BoneID;
				Weights[i] = Weight;
				return;
			}
		}

		// should never get here - more bones than we have space for
		EG_CORE_ASSERT(false, "Too many bones per Vertex");
	}

	void SkinnedMesh::LoadNodeAnim(const aiAnimation* pAnimation)
	{
		for (unsigned int i = 0; i < pAnimation->mNumChannels; i++)
		{
			const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

			std::string name = string(pNodeAnim->mNodeName.data);
			m_NodeAnims[name] = pNodeAnim;
		}
	}

	void SkinnedMesh::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		if (pNodeAnim->mNumScalingKeys == 1)
		{
			Out = pNodeAnim->mScalingKeys[0].mValue;
			return;
		}

		unsigned int ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
		unsigned int NextScalingIndex = (ScalingIndex + 1);
		assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
		float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
		float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
		assert(Factor >= 0.0f && Factor <= 1.0f);
		const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
		const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
		aiVector3D Delta = End - Start;
		Out = Start + Factor * Delta;
	}

	void SkinnedMesh::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		// we need at least two values to interpolate...
		if (pNodeAnim->mNumRotationKeys == 1)
		{
			Out = pNodeAnim->mRotationKeys[0].mValue;
			return;
		}

		unsigned int RotationIndex = FindRotation(AnimationTime, pNodeAnim);
		unsigned int NextRotationIndex = (RotationIndex + 1);
		assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
		float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
		float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
		assert(Factor >= 0.0f && Factor <= 1.0f);
		const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
		const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
		aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
		Out = Out.Normalize();
	}

	void SkinnedMesh::CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		if (pNodeAnim->mNumPositionKeys == 1)
		{
			Out = pNodeAnim->mPositionKeys[0].mValue;
			return;
		}

		unsigned int PositionIndex = FindPosition(AnimationTime, pNodeAnim);
		unsigned int NextPositionIndex = (PositionIndex + 1);
		assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
		float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
		float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
		assert(Factor >= 0.0f && Factor <= 1.0f);
		const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
		const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
		aiVector3D Delta = End - Start;
		Out = Start + Factor * Delta;
	}

	unsigned int SkinnedMesh::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		assert(pNodeAnim->mNumScalingKeys > 0);

		for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++)
		{
			if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime)
			{
				return i;
			}
		}

		assert(0);

		return 0;
	}

	unsigned int SkinnedMesh::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		assert(pNodeAnim->mNumRotationKeys > 0);

		for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++)
		{
			if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime)
			{
				return i;
			}
		}

		assert(0);

		return 0;
	}

	unsigned int SkinnedMesh::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++)
		{
			if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime)
			{
				return i;
			}
		}

		assert(0);

		return 0;
	}

	const aiNodeAnim* SkinnedMesh::FindNodeAnim(const aiAnimation* pAnimation, const string NodeName)
	{
		for (unsigned int i = 0; i < pAnimation->mNumChannels; i++)
		{
			const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

			if (string(pNodeAnim->mNodeName.data) == NodeName)
			{
				return pNodeAnim;
			}
		}

		return NULL;
	}

	void SkinnedMesh::ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const Matrix4f& ParentTransform)
	{
		string NodeName(pNode->mName.data);

		const aiAnimation* pAnimation = m_pScene->mAnimations[0];

		Matrix4f NodeTransformation(pNode->mTransformation);

		//const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);
		const aiNodeAnim* pNodeAnim = m_NodeAnims[NodeName];

		if (pNodeAnim)
		{
			// Interpolate scaling and generate scaling transformation matrix
			aiVector3D Scaling;
			CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
			Matrix4f ScalingM;
			ScalingM.InitScaleTransform(Scaling.x, Scaling.y, Scaling.z);

			// Interpolate rotation and generate rotation transformation matrix
			aiQuaternion RotationQ;
			CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
			Matrix4f RotationM = Matrix4f(RotationQ.GetMatrix());

			// Interpolate translation and generate translation transformation matrix
			aiVector3D Translation;
			CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
			Matrix4f TranslationM;
			TranslationM.InitTranslationTransform(Translation.x, Translation.y, Translation.z);

			// Combine the above transformations
			NodeTransformation = TranslationM * RotationM * ScalingM;
		}

		Matrix4f GlobalTransformation = ParentTransform * NodeTransformation;

		if (m_BoneMapping.find(NodeName) != m_BoneMapping.end())
		{
			unsigned int BoneIndex = m_BoneMapping[NodeName];
			m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].BoneOffset;
		}

		for (unsigned int i = 0; i < pNode->mNumChildren; i++)
		{
			ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
		}
	}

	bool SkinnedMesh::InitFromScene(const aiScene* pScene, const string& Filename)
	{
		m_Entries.resize(pScene->mNumMeshes);
		//m_Textures.resize(pScene->mNumMaterials);

		vector<Vector3f> Positions;
		vector<Vector3f> Normals;
		vector<Vector2f> TexCoords;
		vector<VertexBoneData> Bones;
		vector<unsigned int> Indices;

		unsigned int NumVertices = 0;
		unsigned int NumIndices = 0;

		const aiAnimation* pAnimation = m_pScene->mAnimations[0];
		LoadNodeAnim(pAnimation);

		// Count the number of vertices and indices
		for (unsigned int i = 0; i < m_Entries.size(); i++)
		{
			m_Entries[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
			m_Entries[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;
			m_Entries[i].BaseVertex = NumVertices;
			m_Entries[i].BaseIndex = NumIndices;

			NumVertices += pScene->mMeshes[i]->mNumVertices;
			NumIndices += m_Entries[i].NumIndices;
		}

		// Reserve space in the vectors for the vertex attributes and indices
		Positions.reserve(NumVertices);
		Normals.reserve(NumVertices);
		TexCoords.reserve(NumVertices);
		Bones.resize(NumVertices);
		Indices.reserve(NumIndices);

		// Initialize the meshes in the scene one by one
		for (unsigned int i = 0; i < m_Entries.size(); i++)
		{
			const aiMesh* paiMesh = pScene->mMeshes[i];
			InitMesh(i, paiMesh, Positions, Normals, TexCoords, Bones, Indices);
		}

		if (!InitMaterials(pScene, Filename))
		{
			return false;
		}

		// Generate and populate the buffers with vertex attributes and the indices
		glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0]) * Positions.size(), &Positions[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(POSITION_LOCATION);
		glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * TexCoords.size(), &TexCoords[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(TEX_COORD_LOCATION);
		glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Normals[0]) * Normals.size(), &Normals[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(NORMAL_LOCATION);
		glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[BONE_VB]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Bones[0]) * Bones.size(), &Bones[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(BONE_ID_LOCATION);
		glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
		glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);
		glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

		//return GLCheckError();
		return true;
	}

	void SkinnedMesh::InitMesh(unsigned int MeshIndex, const aiMesh* paiMesh, vector<Vector3f>& Positions, vector<Vector3f>& Normals, vector<Vector2f>& TexCoords, vector<VertexBoneData>& Bones, vector<unsigned int>& Indices)
	{
		const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

		// Populate the vertex attribute vectors
		for (unsigned int i = 0; i < paiMesh->mNumVertices; i++)
		{
			const aiVector3D* pPos = &(paiMesh->mVertices[i]);
			const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
			const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

			Positions.push_back(Vector3f(pPos->x, pPos->y, pPos->z));
			Normals.push_back(Vector3f(pNormal->x, pNormal->y, pNormal->z));
			TexCoords.push_back(Vector2f(pTexCoord->x, pTexCoord->y));
		}

		LoadBones(MeshIndex, paiMesh, Bones);

		// Populate the index buffer
		for (unsigned int i = 0; i < paiMesh->mNumFaces; i++)
		{
			const aiFace& Face = paiMesh->mFaces[i];
			assert(Face.mNumIndices == 3);
			Indices.push_back(Face.mIndices[0]);
			Indices.push_back(Face.mIndices[1]);
			Indices.push_back(Face.mIndices[2]);
		}
	}

	void SkinnedMesh::LoadBones(unsigned int MeshIndex, const aiMesh* pMesh, vector<VertexBoneData>& Bones)
	{
		for (unsigned int i = 0; i < pMesh->mNumBones; i++)
		{
			unsigned int BoneIndex = 0;
			string BoneName(pMesh->mBones[i]->mName.data);

			if (m_BoneMapping.find(BoneName) == m_BoneMapping.end())
			{
				// Allocate an index for a new bone
				BoneIndex = m_NumBones;
				m_NumBones++;
				BoneInfo bi;
				m_BoneInfo.push_back(bi);
				m_BoneInfo[BoneIndex].BoneOffset = pMesh->mBones[i]->mOffsetMatrix;
				m_BoneMapping[BoneName] = BoneIndex;
			}
			else
			{
				BoneIndex = m_BoneMapping[BoneName];
			}

			for (unsigned int j = 0; j < pMesh->mBones[i]->mNumWeights; j++)
			{
				unsigned int VertexID = m_Entries[MeshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
				float Weight = pMesh->mBones[i]->mWeights[j].mWeight;
				Bones[VertexID].AddBoneData(BoneIndex, Weight);
			}
		}
	}

	bool SkinnedMesh::InitMaterials(const aiScene* pScene, const string& Filename)
	{
		// Extract the directory part from the file name
		string::size_type SlashIndex = Filename.find_last_of("/");
		string Dir;

		if (SlashIndex == string::npos)
		{
			Dir = ".";
		}
		else if (SlashIndex == 0)
		{
			Dir = "/";
		}
		else
		{
			Dir = Filename.substr(0, SlashIndex);
		}

		bool Ret = true;

		// Initialize the materials
		for (unsigned int i = 0; i < pScene->mNumMaterials; i++)
		{
			const aiMaterial* pMaterial = pScene->mMaterials[i];

			//m_Textures[i] = NULL;

			if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
			{
				aiString Path;

				if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
				{
					string p(Path.data);

					if (p.substr(0, 2) == ".\\")
					{
						p = p.substr(2, p.size() - 2);
					}

					string FullPath = Dir + "/" + p;

					Ref<Texture2D> tex = Texture2D::Create(FullPath);
					tex->SetType(TextureType::Diffuse);
					m_Textures.push_back(tex);

					if (!tex)
					{
						EG_CORE_ERROR("Error loading texture '{0}'", FullPath.c_str());
						//delete m_Textures[i];
						//m_Textures[i] = NULL;
						Ret = false;
					}
					else
					{
						printf("%d - loaded texture '%s'\n", i, FullPath.c_str());
					}
				}
			}
		}

		return Ret;
	}

	void SkinnedMesh::Clear()
	{
		//for (unsigned int i = 0; i < m_Textures.size(); i++)
		//{
		//	SAFE_DELETE(m_Textures[i]);
		//}

		if (m_Buffers[0] != 0)
		{
			glDeleteBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);
		}

		if (m_VAO != 0)
		{
			glDeleteVertexArrays(1, &m_VAO);
			m_VAO = 0;
		}
	}

}

