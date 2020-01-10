#pragma once

#include "Engine.h"

using namespace Engine;

class Tile
{
private:
	struct TileData
	{
		int Cost;
		std::string Name;
	};

public:
	Tile(int index, const char* texturePath, const char* name, int cost, const glm::vec3& positionOffset = glm::vec3(0.0f), const float& rotationOffset = 0.0f);
	void OnRender(const Ref<Shader>& shader, Model& model);

	inline const int GetIndex() const { return m_Index; }
	Transform& GetTransform() { return m_Transform; }
	const Ref<Texture2D>& GetTexture() inline const { return m_Texture; }
	inline const TileData GetTileData()  const {return m_Data; }

private:

	int m_Index = -1;
	Transform m_Transform;
	Ref<Texture2D> m_Texture;
	TileData m_Data;
};

class Level
{
public:
	Level();
	~Level();

	bool Load(ShaderLibrary* shaderLib);
	void OnUpdate(Timestep ts);



private:
	bool m_Loaded = false;
	std::vector<Tile> m_Tiles;
	Model m_TileModel;
	Ref<Shader> m_TileShader;
	ShaderLibrary* m_ShaderLibrary = nullptr;
};
