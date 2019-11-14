#include "EGpch.h"
#include "Skybox.h"

#include "stb_image.h"
#include "glad/glad.h"

namespace Engine
{
	void Skybox::Load(const std::string& path)
	{
		glGenTextures(1, &m_Texture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture);

		int width, height, nrChannels;
		std::vector<std::string> faces = {
				path + "/right.png",
				path + "/left.png",
				path + "/top.png",
				path + "/bottom.png",
				path + "/front.png",
				path + "/back.png"
		};

		for (unsigned int i = 0; i < faces.size(); i++)
		{
			unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
				);
				stbi_image_free(data);
			}
			else
			{
				EG_CORE_ERROR("Cubemap texture failed to load at path: {0}", faces[i]);
				stbi_image_free(data);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		m_Shader = Shader::Create("assets/shaders/skybox.glsl");

		GLfloat cube_vertices[] = {
			// front
			-1.0, -1.0,  1.0,
			 1.0, -1.0,  1.0,
			 1.0,  1.0,  1.0,
			-1.0,  1.0,  1.0,
			// back
			-1.0, -1.0, -1.0,
			 1.0, -1.0, -1.0,
			 1.0,  1.0, -1.0,
			-1.0,  1.0, -1.0
		};

		unsigned int cube_elements[] = {
			// front
			2, 1, 0, 0, 3, 2,
			// right
			6, 5, 1, 1, 2, 6,
			// back
			5, 6, 7, 7, 4, 5,
			// left
			3, 0, 4, 4, 7, 3,
			// bottom
			1, 5, 4, 4, 0, 1,
			// top
			6, 2, 3, 3, 7, 6
		};

		m_VertexArray = VertexArray::Create();
		Engine::Ref<Engine::VertexBuffer> vb = Engine::VertexBuffer::Create(cube_vertices, sizeof(cube_vertices));
		Engine::Ref<Engine::IndexBuffer> ib = Engine::IndexBuffer::Create(cube_elements, sizeof(cube_elements) / sizeof(unsigned int));
		vb->SetLayout(Engine::BufferLayout{ {Engine::ShaderDataType::Float3, "a_Positions"} });
		m_VertexArray = Engine::VertexArray::Create();
		m_VertexArray->AddVertexBuffer(vb);
		m_VertexArray->SetIndexBuffer(ib);
	}

	void Skybox::Draw(const PerspectiveCamera& camera)
	{
		glDepthMask(GL_FALSE);
		m_Shader->Bind();
		m_Shader->SetInt1("u_SkyboxTexture", 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture);

		m_Shader->SetMat4("u_ViewMatrix", glm::mat4(glm::mat3(camera.GetViewMatrix())));
		m_Shader->SetMat4("u_ProjectionMatrix", camera.GetProjectionMatrix());
		m_VertexArray->Bind();
		glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		m_VertexArray->Unbind();
		glDepthMask(GL_TRUE);
	}

}

