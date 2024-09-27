#include "WarpGate.h"
#include"../MyLib/Physics/ColliderSphere.h"
#include"../Player.h"
#include<EffekseerForDXLib.h>
#include"../SoundManager.h"
#include"../EffectManager.h"
#include"../ScreenManager.h"

namespace
{
	const char* kGaussScreenName = "Gauss";
	const char* kColorScreenName = "Color";
	const char* kBrightScreenName = "HighBright";
}

Effekseer::Matrix43 GetEffMatrix(DxLib::MATRIX mat)
{
	Effekseer::Matrix43 res;
	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 3; ++x)
		{
			res.Value[y][x] = mat.m[y][x];
		}
	}
	return res;
}

namespace
{
	const char* name = "warpGate";
	const char* effectname = "warpEffect.efk";
}

WarpGate::WarpGate(Vec3 pos,int handle):Collidable(Priority::Static, ObjectTag::WarpGate),
m_emitterHandle(EffectManager::GetInstance().GetEffectData(effectname))
{
	AddCollider(MyEngine::ColliderBase::Kind::Sphere);//�����œ��ꂽ�̂͏d�͂̉e���͈�
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	item->radius = 60;
	m_rigid->SetPos(pos);

	m_effectPlayHandle=PlayEffekseer3DEffect(m_emitterHandle);
	m_gaussScreenHandle = ScreenManager::GetInstance().GetScreenData(kGaussScreenName, 1600, 900);
	m_colorScreenHandle = ScreenManager::GetInstance().GetScreenData(kColorScreenName, 1600, 900);
}

WarpGate::~WarpGate()
{
	StopEffekseer3DEffect(m_effectPlayHandle);
	EffectManager::GetInstance().DeleteEffectData(effectname);
}

void WarpGate::Init()
{
}

void WarpGate::Update()
{
	
	
}

void WarpGate::SetEffectPos()
{
	m_upVec = (m_warpPos - m_rigid->GetPos()).GetNormalized();
	
	MATRIX mat ;

	Vec3 axis=Cross(Vec3::Up(), m_upVec);
	mat = MGetRotAxis(axis.VGet(), acos(Dot(Vec3::Up(), m_upVec)));

	mat = MMult(mat,MGetTranslate(Vec3(m_rigid->GetPos()+m_upVec*200).VGet()));
	auto effect = GetEffekseer3DManager();
	effect->SetBaseMatrix(m_effectPlayHandle,GetEffMatrix(mat));
}

void WarpGate::Draw()
{
	//m_screenHandle=GetDrawScreen();
	//SetDrawScreen(m_colorScreenHandle);
	//ClearDrawScreen();
	//DrawLine3D(m_rigid->GetPos().VGet(), ((m_warpPos - m_rigid->GetPos()).GetNormalized() * 300 + m_rigid->GetPos()).VGet(), 0xaaaaff);

	//GraphFilterBlt(m_colorScreenHandle, m_gaussScreenHandle, DX_GRAPH_FILTER_GAUSS, 16,900);
	//
	//SetDrawScreen(m_screenHandle);

	//// �`��u�����h���[�h�����Z�ɂ���
	//SetDrawBlendMode(DX_BLENDMODE_ADD, 255);

	//
	//DrawExtendGraph(0, 0, 1600, 900, m_gaussScreenHandle,true);
	//DrawExtendGraph(0, 0, 1600, 900, m_gaussScreenHandle, true);

	//// �`��u�����h���[�h���u�����h�����ɖ߂�
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	DrawSphere3D(m_rigid->GetPos().VGet(), 60, 7, 0xff00ff, 0xff00ff, false);
	DrawCube3D(Vec3(m_rigid->GetPos() + Vec3(60, 60, 60)).VGet(), Vec3(m_rigid->GetPos() + Vec3(-60, -60, -60)).VGet(), 0xffff00, 0xffff00, false);
}

void WarpGate::OnCollideEnter(std::shared_ptr<Collidable> colider)
{
	if (colider->GetTag() == ObjectTag::Stage)
	{
		m_nowPlanetPos = colider->GetRigidbody()->GetPos();
	}
	if (colider->GetTag() == ObjectTag::Player)
	{
		PlaySoundMem(SoundManager::GetInstance().GetSoundData("boost.mp3"), DX_PLAYTYPE_BACK);
		colider->GetRigidbody()->SetVelocity(Vec3(m_warpPos - colider->GetRigidbody()->GetPos()).GetNormalized() * 100);
		auto player = std::dynamic_pointer_cast<Player>(colider);
		player->m_playerUpdate = &Player::BoostUpdate;
		player->SetBoost();
	}
}
