#include "StarCapture.h"

#include"PlayerSphere.h"
#include"Player.h"
#include"Easing.h"

namespace
{
	constexpr int kCaptureMaxFrame = 200;
}

StarCapture::StarCapture(Vec3 pos, int radius, int captureRadius) : Collidable(Priority::Static, ObjectTag::Player)
{
	m_rigid->SetPos(pos);
	m_radius = radius;
	m_captureRadius = captureRadius;
	SetAntiGravity();
	
	AddCollider(MyEngine::ColliderBase::Kind::Sphere);
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	item->radius = m_radius;
	m_rigid->SetVelocity(Vec3(0, 0, 0));
	m_ratio = 0;
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
	
#ifdef _DEBUG
	for (auto& col : m_colliders)
	{
		auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(col);
		DrawSphere3D(m_rigid->GetPos().VGet(), item->radius, 20, 0x0000ff, 0xffffff, false);
		DrawFormatString(0, 200, 0xddaa00, "Velocity(%f,%f,%f)", m_rigid->GetVelocity().x, m_rigid->GetVelocity().y, m_rigid->GetVelocity().z);
		DrawFormatString(0, 250, 0xddaa00,"Pos(%f,%f,%f)",m_rigid->GetPos().x, m_rigid->GetPos().y, m_rigid->GetPos().z);
	
	}
	
#endif
}

void StarCapture::OnCollideEnter(std::shared_ptr<Collidable> colider)
{
	if (colider->GetTag() == ObjectTag::PlayerBullet)
	{
		AddCollider(MyEngine::ColliderBase::Kind::Sphere);
		m_captureCol = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
		m_captureCol->radius = m_captureRadius;
		m_captureCol->isTrigger = true;

		auto sphere = dynamic_pointer_cast<PlayerSphere>(colider);

		m_player = sphere->m_player;
	}
	if (colider->GetTag() == ObjectTag::Player)
	{
		RemoveCollider(m_captureCol);
		m_player->SetIsCapture(false);

	}
}

void StarCapture::OnTriggerEnter(std::shared_ptr<Collidable> colider)
{
	if (colider->GetTag() == ObjectTag::Player)
	{
		m_playerStartPos = colider->GetRigidbody()->GetPos();
		m_player->SetIsCapture(true);
	}
}

void StarCapture::OnTriggerStay(std::shared_ptr<Collidable> colider)
{
	if (colider->GetTag() == ObjectTag::Player)
	{
		float lenge = (m_rigid->GetPos() - m_player->GetPos()).Length();
		float ratio = ((static_cast<float>(m_captureRadius) -lenge) / static_cast<float>(m_captureRadius));
		m_ratio+=0.002f;
		if (m_ratio > 1)
		{
			m_ratio = 1;
		}
		m_player->SetPos(EaseInOut(m_playerStartPos, m_rigid->GetPos(), m_ratio,2));
	}
}

void StarCapture::OnTriggerExit(std::shared_ptr<Collidable> colider)
{
	if (colider->GetTag() == ObjectTag::Player)
	{
		m_player->SetIsCapture(false);
	}
}

void StarCapture::WaitingUpdate()
{
}

void StarCapture::CaptureUpdate()
{
}
