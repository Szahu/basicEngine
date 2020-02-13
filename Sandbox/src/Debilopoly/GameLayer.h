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

	virtual void OnUpdate(Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Event& event) override;

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

	glm::vec4 dataToSend = glm::vec4(0.0f);
	Transform trShrek;

	Model testModel2;

	rp3d::Vector3 gravity = rp3d::Vector3(0.0f, -9.81f, 0.0f);
	rp3d::DynamicsWorld world;
	long double previousFrameTime = 0.0;
	long double accumulator = 0.0;
	rp3d::Vector3 initPosition = rp3d::Vector3(0.0f, 5.0f, 0.0f);
	rp3d::Quaternion initOrientation = rp3d::Quaternion::identity();
	rp3d::Transform transform = rp3d::Transform(initPosition, initOrientation);
	rp3d::RigidBody* body;
	rp3d::Transform prevTransform;
	rp3d::CapsuleShape shape;
	rp3d::ProxyShape* proxShape;

	rp3d::RigidBody* floor;
	rp3d::BoxShape floor_shape;
	rp3d::ProxyShape* floor_proxShape;

	Model plane;

	bool show = true;
};
