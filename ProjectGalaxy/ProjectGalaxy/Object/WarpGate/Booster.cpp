#include "Booster.h"
#include"ColliderSphere.h"
#include"Player.h"
#include"SoundManager.h"
#include"EffectManager.h"
#include"ScreenManager.h"

namespace
{
	const char* kGaussScreenName = "Gauss";
	const char* kColorScreenName = "Color";
	const char* kBrightScreenName = "HighBright";

	constexpr int kRadius = 20;
}

namespace
{
	const char* name = "booster";
	const char* effectname = "warpEffect.efk";
}

Booster::Booster(Vec3 pos, Vec3 Dir, int handle) :Collidable(Priority::Static, ObjectTag::WarpGate),
m_emitterHandle(EffectManager::GetInstance().GetEffectData(effectname))
{
	m_dir = Dir;
	SetAntiGravity();
	AddCollider(MyEngine::ColliderBase::Kind::Sphere);//ここで入れたのは重力の影響範囲
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	item->radius = kRadius;
	m_rigid->SetPos(pos);

	m_gaussScreenHandle = ScreenManager::GetInstance().GetScreenData(kGaussScreenName, 1600, 900);
	m_colorScreenHandle = ScreenManager::GetInstance().GetScreenData(kColorScreenName, 1600, 900);
}

Booster::~Booster()
{
	
}

void Booster::Init()
{
}

void Booster::Update()
{


}

void Booster::SetEffectPos()
{
	m_upVec = (m_warpPos - m_rigid->GetPos()).GetNormalized();

	MATRIX mat;

	Vec3 axis = Cross(Vec3::Up(), m_upVec);
	mat = MGetRotAxis(axis.VGet(), acos(Dot(Vec3::Up(), m_upVec)));

	mat = MMult(mat, MGetTranslate(Vec3(m_rigid->GetPos() + m_upVec * 200).VGet()));
}

void Booster::Draw()
{
	//m_screenHandle=GetDrawScreen();
	//SetDrawScreen(m_colorScreenHandle);
	//ClearDrawScreen();
	DrawLine3D(m_rigid->GetPos().VGet(),(m_dir*60 + m_rigid->GetPos()).VGet(), 0xff00ff);

	//GraphFilterBlt(m_colorScreenHandle, m_gaussScreenHandle, DX_GRAPH_FILTER_GAUSS, 16,900);
	//
	//SetDrawScreen(m_screenHandle);

	//// 描画ブレンドモードを加算にする
	//SetDrawBlendMode(DX_BLENDMODE_ADD, 255);

	//
	//DrawExtendGraph(0, 0, 1600, 900, m_gaussScreenHandle,true);
	//DrawExtendGraph(0, 0, 1600, 900, m_gaussScreenHandle, true);

	//// 描画ブレンドモードをブレンド無しに戻す
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	DrawSphere3D(m_rigid->GetPos().VGet(), kRadius, 7, 0xff00ff, 0x0000ff, false);
	DrawCube3D(Vec3(m_rigid->GetPos() + Vec3(20, 20, 20)).VGet(), Vec3(m_rigid->GetPos() + Vec3(-20, -20, -20)).VGet(), 0xffff00, 0xffff00, false);
}

void Booster::OnCollideEnter(std::shared_ptr<Collidable> colider)
{
	if (colider->GetTag() == ObjectTag::Stage)
	{
		m_nowPlanetPos = colider->GetRigidbody()->GetPos();
	}
	if (colider->GetTag() == ObjectTag::Player)
	{
		PlaySoundMem(SoundManager::GetInstance().GetSoundData("boost.mp3"), DX_PLAYTYPE_BACK);
		//colider->GetRigidbody()->SetVelocity(Vec3(m_rigid->GetPos()-colider->GetRigidbody()->GetPos()).GetNormalized()*15);
		colider->GetRigidbody()->SetVelocity(m_dir * 15);
		auto player = std::dynamic_pointer_cast<Player>(colider);
		player->m_playerUpdate = &Player::BoostUpdate;
		player->SetBoost();
	}
}
