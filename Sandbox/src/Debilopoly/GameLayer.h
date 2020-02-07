#pragma once

#include "Engine.h"
#include "Engine/Core/Layer.h"

#include "Level.h"

#include "Engine/Toolbox/Audio.h"

#include "Engine/Networking/Socket.h"
#include "Engine/Events/ApplicationEvent.h"

#include "Server.h"
#include "Client.h"

#include "rp3d/src/reactphysics3d.h"

using namespace Engine;

class GameLayer : public Layer
{
public:
	GameLayer();
	~GameLayer() = default;


	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Event& event) override;

	bool OnKey(KeyPressedEvent& e)
	{
		if (e.GetKeyCode() == EG_KEY_ENTER)
		{
			if (paused)
			{
				sound.Play();
				paused = !paused;
			}
			else
			{
				sound.Pause();
				paused = !paused;
			}
			return false;
		}
		return false;
	}

private:
	unsigned int FPS = 0;
	ShaderLibrary m_ShaderLibrary;
	PerspectiveCameraController m_CameraController;
	std::vector<PointLight> m_PointLights;
	std::vector<SpotLight> m_SpotLights;
	Ref<VertexArray> m_ScreenQuad;
	Ref<FrameBuffer> m_FrameBuffer;

	SkinnedMesh testModel;
	Model testModel1;
	Level testLevel;

	Sound sound;
	bool paused = false;

	Server testServer;
	Client testClient;
	glm::vec4 m_Players[4];

	glm::vec4 dataToSend;
	Transform trShrek;

	Model testModel2;

	rp3d::SphereShape* testShape;
};
