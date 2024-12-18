#include "Boss.h"
#include"ColliderSphere.h"
namespace
{
	constexpr float kBodyRadiusSize = 50.f;
	constexpr int kKnockBackFrameMax = 50;

	constexpr float kAnimFrameSpeed = 30.0f;//アニメーション進行速度

	//アニメーションの切り替えにかかるフレーム数
	constexpr float kAnimChangeFrame = 8.0f;
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;

	constexpr float kFrameParSecond = 60.0f;
}
Boss::Boss(Vec3 pos):Enemy(Priority::Boss,ObjectTag::Enemy)
{
	m_rigid->SetPos(pos);
	AddCollider(MyEngine::ColliderBase::Kind::Sphere, ColideTag::Body);
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back()->col);
	item->radius = kBodyRadiusSize;

	m_color = 0xff00ff;
	m_bossUpdate = &Boss::NeutralUpdate;
}

Boss::~Boss()
{
}

void Boss::Init()
{
}

void Boss::Update()
{
}

void Boss::Draw()
{
	DrawSphere3D(m_rigid->GetPos().VGet(), kBodyRadiusSize,8,m_color,0x000000,true);
}

void Boss::InitUpdate()
{
}

void Boss::NeutralUpdate()
{
}

void Boss::AnglyUpdate()
{
}

void Boss::DestroyPlanetUpdate()
{
}

void Boss::KnockBackUpdate()
{
	m_color = 0xff0000;
	m_knockBackFrame++;
	if (m_knockBackFrame > kKnockBackFrameMax)
	{
		m_color = 0xff00ff;
		m_knockBackFrame = 0;
		m_bossUpdate = &Boss::NeutralUpdate;
	}
}


bool Boss::UpdateAnim(int attachNo)
{
	//アニメーションが設定されていないので終了
	if (attachNo == -1) return false;

	//アニメーションを進行させる
	float now = MV1GetAttachAnimTime(m_modelHandle, attachNo);//現在の再生カウント
	now += kAnimFrameSpeed * m_animationSpeed / kFrameParSecond;//アニメーションカウントを進める


	//現在再生中のアニメーションの総カウントを取得する
	float total = MV1GetAttachAnimTotalTime(m_modelHandle, attachNo);
	bool isLoop = false;
	while (now >= total)
	{
		now -= total;
		isLoop = true;
	}

	MV1SetAttachAnimTime(m_modelHandle, attachNo, now);

	return isLoop;
}

void Boss::ChangeAnim(int animIndex, int speed)
{
	m_animationSpeed = speed;
	//さらに古いアニメーションがアタッチされている場合はこの時点で削除しておく
	if (m_prevAnimNo != -1)
	{
		MV1DetachAnim(m_modelHandle, m_prevAnimNo);
	}

	//現在再生中の待機アニメーションは変更前のアニメーション扱いに
	m_prevAnimNo = m_currentAnimNo;

	//変更後のアニメーションとして攻撃アニメーションを改めて設定する
	m_currentAnimNo = MV1AttachAnim(m_modelHandle, animIndex, -1, false);

	//切り替えの瞬間は変更前のアニメーションが再生される状態にする
	m_animBlendRate = 0.0f;

	//変更前のアニメーション100%
	DxLib::MV1SetAttachAnimBlendRate(m_modelHandle, m_prevAnimNo, 1.0f - m_animBlendRate);
	//変更後のアニメーション0%
	DxLib::MV1SetAttachAnimBlendRate(m_modelHandle, m_currentAnimNo, m_animBlendRate);
}

void Boss::OnCollideEnter(std::shared_ptr<Collidable> colider, ColideTag ownTag, ColideTag targetTag)
{
}

void Boss::OnTriggerEnter(std::shared_ptr<Collidable> colider, ColideTag ownTag, ColideTag targetTag)
{

}

