#include "EGpch.h"
#include "ParticleSystem.h"

#include "engine/Toolbox/BasicMeshes.h"
#include "Engine/Renderer/RenderCommand.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/compatibility.hpp"

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"


namespace Engine
{
	ParticleSystem::ParticleSystem()
	{
		m_ParticlePool.resize(m_ParticlesMaxCount);
		m_ParticleShader = Shader::Create("assets/shaders/ParticleSystem/Particle_Color.glsl");
	}

	void ParticleSystem::OnUpdate(Timestep ts)
	{
		//m_Transforms_Colors.swap(std::vector<ParticleData>());
		m_Transforms.swap(std::vector<glm::vec4>());

		for (auto& particle : m_ParticlePool)
		{
			if (!particle.Active) continue;

			if (particle.LifeRemaining <= 0.0f)
			{
				particle.Active = false;
				continue;
			}

			particle.LifeRemaining -= ts;
			particle.Position += glm::vec3(particle.Velocity, 0.001f) * (float)ts * particle.SizeBegin;
			particle.Rotation += 0.01f * ts;

			//Fade away
			float life = particle.LifeRemaining / particle.LifeTime;
			glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
			color.a = color.a * life;

			float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);
			//size = 40.5f;

			//Rendering
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), { particle.Position.x, particle.Position.y, particle.Position.z })
				//* glm::rotate(glm::mat4(1.0f), particle.Rotation, { 0.0f, 0.0f, 1.0f })
				* glm::scale(glm::mat4(1.0f), { size, size, 1.0f });

			glm::vec4 col0 = { transform[0][0], transform[0][1], transform[0][2], transform[0][3] };
			glm::vec4 col1 = { transform[1][0], transform[1][1], transform[1][2], transform[1][3] };
			glm::vec4 col2 = { transform[2][0], transform[2][1], transform[2][2], transform[2][3] };
			glm::vec4 col3 = { transform[3][0], transform[3][1], transform[3][2], transform[3][3] };

			m_Transforms.push_back(col0);
			m_Transforms.push_back(col1);
			m_Transforms.push_back(col2);
			m_Transforms.push_back(col3);
			m_Transforms.push_back(color);


		}
	}

	void ParticleSystem::OnRender()
	{
		if (!m_QuadVA)
		{
			m_QuadVA = BasicMeshes::Quad();
			Ref<VertexBuffer> buf = VertexBuffer::Create(nullptr, m_ParticlesMaxCount * sizeof(glm::vec3));
			buf->SetLayout({
				{ShaderDataType::Float4, "col0"},
				{ShaderDataType::Float4, "col1"},
				{ShaderDataType::Float4, "col2"},
				{ShaderDataType::Float4, "col3"},
				{ShaderDataType::Float4, "Color"}
				});
			m_QuadVA->AddVertexBuffer(buf, true, 1);
		}
		m_ParticleShader->Bind();
		m_QuadVA->Bind();

		/*
		for (auto& particle : m_ParticlePool)
		{
			if (!particle.Active) continue;

			//Fade away
			float life = particle.LifeRemaining / particle.LifeTime;
			glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
			color.a = color.a * life;
			
			float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);
			//size = 40.5f;
			
			//Rendering
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), { particle.Position.x, particle.Position.y, particle.Position.z })
				//* glm::rotate(glm::mat4(1.0f), particle.Rotation, { 0.0f, 0.0f, 1.0f })
				* glm::scale(glm::mat4(1.0f), { size, size, 1.0f });
				
			
			m_ParticleShader->SetMat4("u_Transform", transform);
			m_ParticleShader->SetFloat4("u_Color", color);
			RenderCommand::DrawIndexed(m_QuadVA);

		}
		*/

		m_QuadVA->GetVertexBuffers()[1]->Bind();
		//if (m_Transforms_Colors.size() != 0)
		//{
		//	glBufferSubData(GL_ARRAY_BUFFER, 0, m_Transforms_Colors.size() * sizeof(ParticleData), &m_Transforms_Colors[0]);
		//}
		if (m_Transforms.size() != 0)
		{
			glBufferSubData(GL_ARRAY_BUFFER, 0, m_Transforms.size() * sizeof(glm::vec4), &m_Transforms[0]);
		}
		m_QuadVA->GetVertexBuffers()[1]->Unbind();
		RenderCommand::DrawInstanced(m_QuadVA, m_Transforms.size());

		m_ParticleShader->Unbind();
		m_QuadVA->Unbind();
	}

	void ParticleSystem::Emit(const ParticleProps& props)
	{
		Particle& particle = m_ParticlePool[m_PoolIndex];
		particle.Active = true;
		particle.Position = props.Position;
		particle.Rotation = Random::Float() * 2.0f * glm::pi<float>();

		// Velocity
		particle.Velocity = props.Velocity;
		particle.Velocity.x += props.VelocityVariation.x * (Random::Float() - 0.5f);
		particle.Velocity.y += props.VelocityVariation.y * (Random::Float() - 0.5f);

		// Color
		particle.ColorBegin = props.ColorBegin;
		particle.ColorEnd = props.ColorEnd;

		particle.LifeTime = props.LifeTime;
		particle.LifeRemaining = props.LifeTime;
		particle.SizeBegin = props.SizeBegin + props.SizeVariation * (Random::Float() - 0.5f);
		particle.SizeEnd = props.SizeEnd;

		m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
	}

}

