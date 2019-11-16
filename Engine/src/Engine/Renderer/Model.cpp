#include "EGpch.h"
#include "Model.h"

#include "glad/glad.h"

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

				switch (typeName)
				{
					case TextureType::Diffuse:
						Diffs++;
						m_DiffTextures.push_back(tex);
						break;

					case TextureType::Specular:
						Specs++;
						m_SpecTextures.push_back(tex);
						break;

					case TextureType::Normal:
						Norms++;
						m_NormTextures.push_back(tex);
						break;
				}
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
			unsigned int DiffTosend = 0;
			unsigned int SpecTosend = 0;
			unsigned int NormTosend = 0;

			if (meshes[i].HasTextureOfType(TextureType::Diffuse)) { DiffTosend = DiffsUsed; DiffsUsed++; }
			else DiffTosend = -1;

			if (meshes[i].HasTextureOfType(TextureType::Specular)) { SpecTosend = SpecsUsed + 6; SpecsUsed++; }
			else SpecTosend = -1;

			if (meshes[i].HasTextureOfType(TextureType::Normal)) { NormTosend = NormsUsed + 12; NormsUsed++; }
			else NormTosend = -1;


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

				vertexBufferData.push_back(DiffTosend);
				vertexBufferData.push_back(SpecTosend);
				vertexBufferData.push_back(NormTosend);

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
			{ShaderDataType::Int3, "a_TexSamples"},
			{ShaderDataType::Float3, "a_Tangents"},
			{ShaderDataType::Float3, "a_BiTangents"}
		});

		Ref<IndexBuffer> ib = IndexBuffer::Create(&indexBufferData[0], indexBufferData.size());

		m_FinalVA->AddVertexBuffer(vb);
		m_FinalVA->SetIndexBuffer(ib);
	}

	void Model::ProccesTextures(const Ref<Shader>& shader)
	{
		for (int i = 0; i < m_DiffTextures.size(); i++)
		{
			m_DiffTextures[i]->Bind(i);
			shader->SetInt1("u_texture_diffuse_" + std::to_string(i), i);
		}
		for (int i = 0; i < m_SpecTextures.size(); i++)
		{
			m_SpecTextures[i]->Bind(i + 6);
			shader->SetInt1("u_texture_specular_" + std::to_string(i), i + 6);
		}
		for (int i = 0; i < m_NormTextures.size(); i++)
		{
			m_NormTextures[i]->Bind(i + 12);
			shader->SetInt1("u_texture_diffuse_" + std::to_string(i), i + 12);
		}
		
	}

}

