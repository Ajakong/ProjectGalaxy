#pragma once
#include "Collidable.h"
using namespace MyEngine;

class Player;
class ColliderSphere;

class StarCapture : public Collidable
{
public:
	StarCapture(Vec3 pos,int radius=30,int captureRadius=500);
	virtual ~StarCapture();

	void Init();
	void Update();
	void Draw();

	virtual void OnCollideEnter(std::shared_ptr<Collidable> colider);
	virtual void OnTriggerEnter(std::shared_ptr<Collidable> colider);
	virtual void OnTriggerStay(std::shared_ptr<Collidable> colider);
	virtual void OnTriggerExit(std::shared_ptr<Collidable> colider);
private:
	using StarCaptureState_t = void(StarCapture::*)();
	StarCaptureState_t m_stateUpdate;

	void WaitingUpdate();
	void CaptureUpdate();


private:
	float m_ratio;
	int m_modelhandle;
	int m_radius;
	int m_captureRadius;
	std::shared_ptr<MyEngine::ColliderSphere> m_captureCol;
	std::shared_ptr<Collidable> m_target;
	std::shared_ptr<Player> m_player;
	Vec3 m_playerStartPos;
};

