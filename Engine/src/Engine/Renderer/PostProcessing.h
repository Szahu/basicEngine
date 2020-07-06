#pragma once

#include "Engine/Renderer/FrameBuffer.h"

namespace Engine
{
	struct PostProcessingEffect
	{
	public:

		PostProcessingEffect(const char* libKey);

		void Procces();

	private:
		Ref<FrameBuffer> m_FBO;
		const char* m_ShaderKey;
	};
}

