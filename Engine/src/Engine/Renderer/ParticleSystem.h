#pragma once

#include "glm/glm.hpp"
#include "Engine/Toolbox/Random.h"
#include "engine/Core/Timestep.h"
#include "Engine/Toolbox/PerspectiveCamera.h"
#include "Engine/Renderer/VertexArray.h"

namespace Engine
{

	struct ParticleProps
	{
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec2 Velocity = glm::vec2(0.0f);
		glm::vec2 VelocityVariation = glm::vec2(5.0f);
		glm::vec4 ColorBegin, ColorEnd;
		float SizeBegin = 1.0f;
		float SizeEnd = 0.0f;
		float SizeVariation = 1.0f;
		float LifeTime = 1.0f;
	};

	class ParticleSystem
	{
	public:

		ParticleSystem();

		void OnUpdate(Timestep ts);
		void OnRender();

		void Emit(const ParticleProps& props);

	private:
		struct Particle
		{
			glm::vec3 Position;
			glm::vec2 Velocity;
			glm::vec4 ColorBegin, ColorEnd;
			float Rotation = 0.0f;
			float SizeBegin, SizeEnd;

			float LifeTime = 4.0f;
			float LifeRemaining = 0.0f;

			bool Active = false;
		};

		uint32_t m_ParticlesMaxCount = 500;

		std::vector<Particle> m_ParticlePool;
		uint32_t m_PoolIndex = m_ParticlesMaxCount - 1;

		Ref<VertexArray> m_QuadVA;
		Ref<Shader> m_ParticleShader;

		std::vector<glm::vec4> m_Transforms;
	};

}