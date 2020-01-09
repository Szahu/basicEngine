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
	Tile(int index, const char* texturePath, const char* name, int cost);
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

	void Load();
	void OnUpdate(Timestep ts);



private:
	std::vector<Tile> m_Tiles;
	
};
