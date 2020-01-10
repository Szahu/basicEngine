#include "Level.h"

using namespace Engine;

Tile::Tile(int index, const char* texturePath, const char* name, int cost, const glm::vec3& positionOffset, const float& rotationOffset)
{
	m_Index = index;
	m_Texture = Texture2D::Create(texturePath);
	m_Data.Name = name;
	m_Data.Cost = cost;
	m_Transform.Translate(positionOffset);
	m_Transform.Rotate(glm::vec3(0.0f, rotationOffset, 0.0f));
}

void Tile::OnRender(const Ref<Shader>& shader, Model& model)
{
	m_Texture->Bind();
	shader->SetMat4("u_Transform", m_Transform);
	model.GetMeshes()[0].GetVertexArray()->Bind();
	RenderCommand::DrawIndexed(model.GetMeshes()[0].GetVertexArray());
	model.GetMeshes()[0].GetVertexArray()->Unbind();
}

Level::Level()
{

}

Level::~Level()
{

}

bool Level::Load(ShaderLibrary* shaderLib)
{
	m_ShaderLibrary = shaderLib;
	m_TileShader = Shader::Create("assets/game/shaders/TileShader.glsl");
	m_TileModel.loadModel("assets/game/models/tiles.obj");

	m_Tiles.push_back(Tile((int)m_Tiles.size(),"assets/game/models/tile_placeholder_tex.jpg", "testname", 6969, { 1.0f, 0.0f, 1.0f }, 90.0f));

	m_Loaded = true;
	return true;
}

void Level::OnUpdate(Timestep ts)
{
	if (!m_Loaded)
	{
		EG_ERROR("Level not loaded!");
		return;
	}

	m_TileShader->Bind();
	m_TileShader->SetInt1("texture_diffuse1", 0);

	for (int i = 0; i < m_Tiles.size(); i++) m_Tiles[i].OnRender(m_TileShader, m_TileModel);

}
