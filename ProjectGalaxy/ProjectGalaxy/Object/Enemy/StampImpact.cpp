#include "StampImpact.h"
#include"Quaternion.h"
namespace
{
	constexpr float kSegment = 20;
}

StampImpact::StampImpact(Vec3 pos, float planetRadius, Vec3 dir) :
	m_pos(pos),
	m_radiusMax(planetRadius),
	m_dir(dir),
	m_nowRadius(0),
	m_deleteFlag(false)
{
	m_impactUpdate = &StampImpact::ExpansionUpdate;
}

StampImpact::~StampImpact()
{
}

void StampImpact::Init()
{
}

void StampImpact::Update()
{
	//直径分進んだら削除
	//移動総数は2Rになり、拡大するのがR、縮小するのもRなので拡大縮小変化総量は2Rとなり
	//1フレームごとの位置変化量と拡大縮小の変化量は等しくなる
	m_pos += m_dir;
	(this->*m_impactUpdate)();
}

void StampImpact::Draw()
{
	Draw3DCircle(m_pos, m_nowRadius, kSegment);
	DrawSphere3D(m_pos.VGet(),3,20,0xff0000, 0xff0000,true);
}

void StampImpact::Draw3DCircle(Vec3 center, float radius, int num_segments)
{
	// 円の中心からのベクトルを法線として正規化
	Vec3 normal = m_dir.GetNormalized(); // 向きのベクトルを正規化
	// 円の法線ベクトルに垂直な2つの直交ベクトルを求める
	Vec3 tangent1 = GetPerpendicular(normal);  // 向きに垂直なベクトル
	Vec3 tangent2 =Cross(normal,tangent1);     // 2つ目の直交ベクトル

	// 2つの直交ベクトルを使って円周を描画
	for (int i = 0; i < num_segments; ++i)
	{
		// 円周の各点を計算
		float theta1 = 2.0f * DX_PI_F * i / num_segments;
		float theta2 = 2.0f * DX_PI_F * (i + 1) / num_segments;

		Vec3 pos1, pos2;

		// 円周上の2点
		pos1 = center + (tangent1* cosf(theta1) + tangent2* sinf(theta1))* radius;
		pos2 = center + (tangent1* cosf(theta2) + tangent2* sinf(theta2))* radius;

		// 3D空間に円を描画
		DrawLine3D(pos1.VGet(), pos2.VGet(), GetColor(255, 0, 0)); // 赤色で円周を描画
	}
}

void StampImpact::ExpansionUpdate()
{
	m_nowRadius++;
	if (m_nowRadius > m_radiusMax)
	{
		m_impactUpdate = &StampImpact::ReductionUpdate;
	}
}

void StampImpact::ReductionUpdate()
{
	m_nowRadius--;
	if (m_nowRadius < 0)
	{
		m_deleteFlag = true;
	}
}
