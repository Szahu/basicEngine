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
	void Mesh::Draw(const Engine::Ref<Engine::Shader>& shader)
	{
		
		// bind appropriate textures
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;

 		for (unsigned int i = 0; i < textures.size(); i++)
		{
			//textures[i]->Bind(0x84C0 + i);
			glActiveTexture(GL_TEXTURE0 + i); 
			// retrieve texture number (the N in diffuse_textureN)
			std::string number;
			std::string name = textures[i]->GetType();
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++); // transfer unsigned int to stream
			else if (name == "texture_normal")
				number = std::to_string(normalNr++); // transfer unsigned int to stream
			else if (name == "texture_height")
				number = std::to_string(heightNr++); // transfer unsigned int to stream

														// now set the sampler to the correct texture unit
			shader->SetInt1((name + number).c_str(), i);
			// and finally bind the texture
			glBindTexture(GL_TEXTURE_2D, textures[i]->GetID());

			//textures[i]->Bind(0x84C0 + i);
		}
		
		if (!HasTextures)
		{
			shader->SetFloat3("u_Material.diffuse", m_material.m_Diffuse);
			shader->SetFloat3("u_Material.ambient", m_material.m_Ambient);
			shader->SetFloat3("u_Material.specular", m_material.m_Specular);
			shader->SetFloat1("u_Material.shininess", m_material.m_Shininess);
		}
	
		
		m_VertexArray->Bind();
		RenderCommand::DrawIndexed(m_VertexArray);
		m_VertexArray->Unbind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);

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
			//textures[i]->Bind(0x84C0 + i);
			glActiveTexture(GL_TEXTURE0 + i);
			// retrieve texture number (the N in diffuse_textureN)
			std::string number;
			std::string name = textures[i]->GetType();
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++); // transfer unsigned int to stream
			else if (name == "texture_normal")
				number = std::to_string(normalNr++); // transfer unsigned int to stream
			else if (name == "texture_height")
				number = std::to_string(heightNr++); // transfer unsigned int to stream

														// now set the sampler to the correct texture unit
			shader->SetInt1((name + number).c_str(), i);
			// and finally bind the texture
			glBindTexture(GL_TEXTURE_2D, textures[i]->GetID());

			//textures[i]->Bind(0x84C0 + i);
		}

		if (!HasTextures)
		{
			shader->SetFloat3("u_Material.diffuse", m_material.m_Diffuse);
			shader->SetFloat3("u_Material.ambient", m_material.m_Ambient);
			shader->SetFloat3("u_Material.specular", m_material.m_Specular);
			shader->SetFloat1("u_Material.shininess", m_material.m_Shininess);
		}
	}

}


