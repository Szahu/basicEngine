#include "Level.h"

using namespace Engine;

Tile::Tile(int index, const char* texturePath, const char* name, int cost)
{
	m_Index = index;
	m_Texture = Texture2D::Create(texturePath);
	m_Data.Name = name;
	m_Data.Cost = cost;
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

void Level::Load()
{

}

void Level::OnUpdate(Timestep ts)
{

}
