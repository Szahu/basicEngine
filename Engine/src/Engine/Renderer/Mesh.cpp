#include "EGpch.h"
#include "Engine/Renderer/Mesh.h"

#include "Engine/Renderer/RenderCommand.h"

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

		Ref<VertexBuffer> t_VBO = VertexBuffer::Create(vertices, vertices.size() * sizeof(Vertex));
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
			textures[i]->Bind(0x84C0 + i);
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
			shader->Bind();
			shader->SetInt1((name + number).c_str(), i);
			// and finally bind the texture
			textures[i]->Bind();
		}
		
		shader->Bind();
		shader->SetFloat3("u_Material.diffuse", m_material.Diffuse);
		shader->SetFloat3("u_Material.ambient", m_material.Ambient);
		shader->SetFloat3("u_Material.specular", m_material.Specular);
		shader->SetFloat1("u_Material.shininess", m_material.Shininess);

		//shader->SetFloat3("u_Light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		//shader->SetFloat3("u_Light.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		//shader->SetFloat3("u_Light.specular", glm::vec3(0.5f, 0.5f, 0.5f));
		// draw mesh
		//glBindVertexArray(VAO);
		m_VertexArray->Bind();
		//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		RenderCommand::DrawIndexed(m_VertexArray);
		m_VertexArray->Unbind();
		// always good practice to set everything back to defaults once configured.
		//textures[0]->Unbind();
	}

}


