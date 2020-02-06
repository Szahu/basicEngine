#pragma once

#include "Engine.h"
#include "Engine/Core/Layer.h"

#include "Level.h"

#include "Engine/Toolbox/Audio.h"

#include "Engine/Networking/Socket.h"
#include "Engine/Events/ApplicationEvent.h"

#include "Server.h"

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

	void SendDataToServer();
	//void ListenOnServer();

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

	//SocketData ServerHint;
	//Socket testServer;
	//char buffer[1024];
	//std::thread serverThread;
	//
	SocketData ClientData;
	Socket Client;
	std::thread clientThread;
	SocketData serverData;
	//
	//bool m_ServerRun = true;
	bool m_ClientRun = true;
	//
	unsigned int myId;
	//
	//bool m_RunServer = false;
	//
	//struct ClientType
	//{
	//	ClientType(unsigned int id) { ID = id; }
	//	glm::vec3 pos = glm::vec3(0.0f);
	//	unsigned int ID;
	//};
	//
	//std::vector<ClientType> m_Clients;
	//
	glm::vec3 m_Players[4];

	Transform trShrek;

	Model testModel2;
};
