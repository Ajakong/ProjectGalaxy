#pragma once
#include "Collidable.h"
using namespace MyEngine;

class StarCapture : public Collidable
{
public:
	StarCapture(Vec3 pos);
	virtual ~StarCapture();

	void Init();
	void Update();
	void Draw();

	virtual void OnCollideEnter(std::shared_ptr<Collidable> colider);
private:
	using StarCaptureState_t = void(StarCapture::*)();
	StarCaptureState_t m_stateUpdate;

	void WaitingUpdate();
	void CaptureUpdate();


private:
	int m_modelhandle;

	std::shared_ptr<Collidable> m_target;

};

