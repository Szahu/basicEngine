#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Renderer/VertexArray.h"
#include "Engine/Toolbox/PerspectiveCamera.h"

namespace Engine
{
	class Skybox
	{
	public:
		void Load(const std::string& path);

		uint32_t GetTexture() const { return m_Texture; }
		const Ref<VertexArray>& GetVertexArray() const { return m_VertexArray; }

		void Draw(const PerspectiveCamera& camera);

	private:
		Ref<VertexArray> m_VertexArray;
		Ref<Shader> m_Shader;
		uint32_t m_Texture;
	};
}