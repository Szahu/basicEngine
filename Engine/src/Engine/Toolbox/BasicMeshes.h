#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/VertexArray.h"

namespace Engine
{
	class BasicMeshes
	{
	public:
		static const Ref<VertexArray> Cube();
		static const Ref<VertexArray> Quad();
		static const Ref<VertexArray> Quad2D();
		static const Ref<VertexArray> ScreenQuad();

	};
}