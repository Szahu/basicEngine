#pragma once

#include <string>
#include "Engine/Core/Core.h"


namespace Engine
{
	enum class TextureType
	{
		None = 0, Diffuse, Specular, Normal, Height
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() = 0;
		virtual uint32_t GetHeight() = 0;
		
		virtual void SetData(void* data, uint32_t size) = 0;

		virtual const uint32_t GetID() const = 0;

		virtual const TextureType& GetType() const = 0;
		virtual const void SetType(const TextureType& type) = 0;

		virtual const std::string& GetPath() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void Unbind() const = 0;

	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(uint32_t width, uint32_t height);
		static Ref<Texture2D> Create(const std::string& path, bool flipOnLoad = false);
	};
}