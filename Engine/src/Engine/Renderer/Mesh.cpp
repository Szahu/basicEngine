#include "EGpch.h"
#include "Engine/Renderer/Mesh.h"

#include "Engine/Renderer/RenderCommand.h"
#include "glad/glad.h"

namespace Engine
{
	Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Ref<Texture2D>> textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;
		this->HasTextures = true;

		setupMesh();
	}

	Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, Material material)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->m_material = material;
		this->HasTextures = false;

		setupMesh();
	}

	void Mesh::setupMesh()
	{
		m_VertexArray = VertexArray::Create();

		Ref<VertexBuffer> t_VBO = VertexBuffer::Create(&vertices[0].Position.x, vertices.size() * sizeof(Vertex));
		t_VBO->SetLayout(BufferLayout{
			{ShaderDataType::Float3, "a_Positions"},
			{ShaderDataType::Float3, "a_Normals"},
			{ShaderDataType::Float2, "a_TexCoords"},
			{ShaderDataType::Float3, "a_Tangents"},
			{ShaderDataType::Float3, "a_BiTangents"}
		});

		Ref<IndexBuffer> t_EBO = IndexBuffer::Create(&indices[0], indices.size());

		m_VertexArray->AddVertexBuffer(t_VBO);
		m_VertexArray->SetIndexBuffer(t_EBO);

		m_VertexArray->Unbind();
	}


	void Mesh::ProccessMaterial(const Engine::Ref<Engine::Shader>& shader)
	{
		// bind appropriate textures
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;

		for (unsigned int i = 0; i < textures.size(); i++)
		{
			// retrieve texture number (the N in diffuse_textureN)
			std::string number;
			TextureType type = textures[i]->GetType();

			switch (type)
			{
				case TextureType::Diffuse: 
					number = std::to_string(diffuseNr++);
					shader->SetInt1(("texture_diffuse" + number).c_str(), i);
					break;

				case TextureType::Specular: 
					number = std::to_string(specularNr++); 
					shader->SetInt1(("texture_specular" + number).c_str(), i);
					break;

				case TextureType::Normal: 
					number = std::to_string(normalNr++);
					shader->SetInt1(("texture_normal" + number).c_str(), i);
					break;

				case TextureType::Height:
					number = std::to_string(heightNr++); 
					shader->SetInt1(("texture_height" + number).c_str(), i);
					break;
			}

			textures[i]->Bind(i);
		}

		if (!HasTextures)
		{
			shader->SetFloat3("u_Material.diffuse", m_material.m_Diffuse);
			shader->SetFloat3("u_Material.ambient", m_material.m_Ambient);
			shader->SetFloat3("u_Material.specular", m_material.m_Specular);
			shader->SetFloat1("u_Material.shininess", m_material.m_Shininess);
		}
	}

	void Mesh::UnbindTextures()
	{
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			textures[i]->Unbind();
		}
	}

	bool Mesh::HasTextureOfType(TextureType type)
	{
		if (!HasTextures) return false;
		else 
		{
			for (int i = 0; i < textures.size(); i++)
			{
				if (textures[i]->GetType() == type) return true;
			}
			return false;
		}

		return false;
	}

}


