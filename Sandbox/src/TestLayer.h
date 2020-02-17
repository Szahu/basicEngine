#pragma once

#include "Engine.h"
#include "Engine/Core/Layer.h"

#include "rp3d/src/reactphysics3d.h"


using namespace Engine;

class TestLayer : public Engine::Layer
{
public:
	TestLayer();
	~TestLayer() = default;


	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Engine::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Engine::Event& event) override;

private:
	ShaderLibrary m_ShaderLibrary;
	PerspectiveCameraController m_CameraController;
	PerspectiveCamera m_Camera;
	std::vector<PointLight> m_PointLights;
	std::vector<SpotLight> m_SpotLights;
	Ref<VertexArray> m_ScreenQuad;
	Ref<FrameBuffer> m_FrameBuffer;
private:
	Model m_Terrain;
	Transform tr_Terrain;
	Model m_Plane;
	Transform m_PlaneTransform;

	rp3d::Vector3 gravity = rp3d::Vector3(0.0f, -0.8f, 0.0f);
	rp3d::DynamicsWorld m_World;

	long double previousFrameTime = 0.0;
	long double accumulator = 0.0;


	rp3d::Vector3 initPosition = rp3d::Vector3(0.0f, 5.0f, 0.0f);
	rp3d::Quaternion initOrientation = rp3d::Quaternion::identity();
	rp3d::Transform transform = rp3d::Transform(initPosition, initOrientation);
	//rp3d::RigidBody* m_PlaneRb;
	rp3d::Transform prevTransform;

	Transform m_CameraTransform;

	glm::vec2 m_Angles = { 0.0f, 0.0f };
};