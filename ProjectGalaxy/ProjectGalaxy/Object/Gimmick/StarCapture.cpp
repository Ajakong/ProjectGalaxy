#include "StarCapture.h"

StarCapture::StarCapture(Vec3 pos) : Collidable(Priority::Static, ObjectTag::Player)
{
}

StarCapture::~StarCapture()
{
}

void StarCapture::Init()
{
}

void StarCapture::Update()
{
}

void StarCapture::Draw()
{
	MV1DrawModel(m_modelhandle);
}

void StarCapture::OnCollideEnter(std::shared_ptr<Collidable> colider)
{
}

void StarCapture::WaitingUpdate()
{
}

void StarCapture::CaptureUpdate()
{
}
