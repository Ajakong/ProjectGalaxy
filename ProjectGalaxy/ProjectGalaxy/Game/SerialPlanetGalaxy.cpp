#include "SerialPlanetGalaxy.h"
#include"DxLib.h"
// EffekseerForDXLib.hをインクルードします。
#include "EffekseerForDXLib.h"
#include"Camera.h"
#include"Physics.h"
#include"Player.h"
#include"WarpGate.h"
#include"Booster.h"
#include"BossPlanet.h"
#include"Takobo.h"
#include"KillerTheSeeker.h"
#include"Gorori.h"
#include"Item.h"
#include"ClearObject.h"
#include<cassert>
#include"Pad.h"
#include"SoundManager.h"
#include"GraphManager.h"
#include"FontManager.h"
#include"ScreenManager.h"
#include"ModelManager.h"
#include"Game.h"

using namespace std;

namespace
{
	//UI
	//ミッション
	constexpr int kUiText_SrkX = 150;
	constexpr int kUiText_SrkY = 170;
	constexpr int kUiText_Width = 1050;
	constexpr int kUiText_Height = 450;
	constexpr float kUiText_Exrate = 0.2f;

	//HPバーの枠
	constexpr int kUiHpbarFrame_PosX = 150;
	constexpr int kUiHpbarFrame_PosY = 35;
	constexpr int kUiHpbarFrame_SrkX = 2600;
	constexpr int kUiHpbarFrame_SrkY = 2560;
	constexpr int kUiHpbarFrame_Width = 1000;
	constexpr int kUiHpbarFrame_Height = 144;
	constexpr float kUiHpbarFrame_Exrate = 0.3f;

	//ChargeRemainTimeのバー
	constexpr int kUiCRT_PosX = 20;
	constexpr int kUiCRT_PosY = 70;
	constexpr int kUiCRT_SrkX = 2655;
	constexpr int kUiCRT_SrkY = 2720;
	constexpr int kUiCRT_Width = 30;
	constexpr int kUiCRT_DisX = 9;
	constexpr int kUiCRT_Height = 80;
	constexpr float kUiCRT_Exrate = 0.3f;

	//HPバー
	constexpr int kUiHpbar_PosX = 15;
	constexpr int kUiHpbar_PosY = 25;
	constexpr int kUiHpbar_Height = 23;
	constexpr float kUiHpbar_mag = 5.35f;//HPに掛けたらいい感じになる倍率

	//タイマーの枠
	constexpr int kUiTimeCountFrame_PosX = 1400;
	constexpr int kUiTimeCountFrame_PosY = 100;
	constexpr int kUiTimeCountFrame_SrkX = 1280;
	constexpr int kUiTimeCountFrame_SrkY = 130;
	constexpr int kUiTimeCountFrame_Width = 1040;
	constexpr int kUiTimeCountFrame_Height = 410;
	constexpr float kUiTimeCountFrame_Exrate = 0.3f;

	//タイマー
	constexpr int kUiTimeCount_PosX = 1350;
	constexpr int kUiTimeCount_PosY = 90;

	//カメラ
	constexpr float kCameraDistanceFront = 800.f;
	constexpr float kCameraDistanceAddFrontInJump = 300.f;
	constexpr float kCameraDistanceUp = 500.f;


	const char* kMiniMapScreenName = "MiniMap";
}

SerialPlanetGalaxy::SerialPlanetGalaxy(std::shared_ptr<Player> playerPointer) : Galaxy(playerPointer)
{
	booster.push_back(make_shared<Booster>(Vec3(0,150,0),Vec3(0,1,1).GetNormalized(), -1));
	MyEngine::Physics::GetInstance().Entry(booster.back());
	booster.push_back(make_shared<Booster>(Vec3(0, -200, 530), Vec3(0,1,0).GetNormalized(), -1));
	MyEngine::Physics::GetInstance().Entry(booster.back());
	camera = make_shared<Camera>();
	planet.push_back(std::make_shared<SpherePlanet>(Vec3(0, -500, 0), 0xaadd33, 3, ModelManager::GetInstance().GetModelData("Sphere/planet_moon.mv1")));
	m_skyDomeH = ModelManager::GetInstance().GetModelData("Skybox.mv1");

	MV1SetScale(m_skyDomeH, VGet(1.3f, 1.3f, 1.3f));

	m_managerUpdate = &SerialPlanetGalaxy::GamePlayingUpdate;
	m_managerDraw = &SerialPlanetGalaxy::GamePlayingDraw;

	m_miniMapScreenHandle = ScreenManager::GetInstance().GetScreenData(kMiniMapScreenName, Game::kScreenWidth, Game::kScreenHeight);
}

SerialPlanetGalaxy::~SerialPlanetGalaxy()
{
	planet.clear();
	takobo.clear();
	gorori.clear();
	poworStone.clear();
	warpGate.clear();
}

void SerialPlanetGalaxy::Init()
{
	SetGlobalAmbientLight(GetColorF(0.0f, 0.0f, 1.0f, 1.0f));

	player->SetMatrix();//モデルに行列を反映

	// 深度値記録バッファ用RT
	DxLib::SetCreateGraphChannelBitDepth(32);
	DxLib::SetCreateDrawValidGraphChannelNum(1);

	MyEngine::Physics::GetInstance().Entry(player);//物理演算クラスに登録
	for (auto& item : planet)
	{
		item->Init();
		MyEngine::Physics::GetInstance().Entry(item);//物理演算クラスに登録
	}
}

void SerialPlanetGalaxy::Update()
{
	(this->*m_managerUpdate)();
}

void SerialPlanetGalaxy::Draw()
{
	(this->*m_managerDraw)();
}

void SerialPlanetGalaxy::IntroUpdate()
{
}

void SerialPlanetGalaxy::IntroDraw()
{
}

void SerialPlanetGalaxy::GamePlayingUpdate()
{
	player->Update();
	if (player->OnAiming())
	{
		camera->Update(player->GetShotDir());
	}
	else
	{
		camera->Update(player->GetPos());
	}
	
	Vec3 planetToPlayer = player->GetPos() - player->GetNowPlanetPos();
	Vec3 sideVec = player->GetSideVec();
	Vec3 front =player->GetFrontVec();//-1をかけて逆ベクトルにしている

	//相対的な軸ベクトルの設定
	
	player->SetUpVec(planetToPlayer);

	camera->SetBoost(player->GetBoostFlag());
	//本当はカメラとプレイヤーの角度が90度以内になったときプレイヤーの頭上を見たりできるようにしたい。
	camera->SetUpVec(player->GetNormVec());

	if (player->GetBoostFlag())
	{
		camera->SetCameraPoint(player->GetPos() + (Vec3(GetCameraUpVector()).GetNormalized() * (kCameraDistanceUp - 400) - front * ((kCameraDistanceFront - 700) + kCameraDistanceAddFrontInJump * player->GetJumpFlag())));
	}
	else
	{
		if (player->OnAiming())
		{
			camera->SetCameraPoint(player->GetPos()+ player->GetShotDir() *- 50+player->GetNormVec()*80+player->GetSideVec()*20);
		}
		else
		{
			camera->SetCameraPoint(player->GetPos() + (Vec3(GetCameraUpVector()).GetNormalized() * kCameraDistanceUp - front * (kCameraDistanceFront + kCameraDistanceAddFrontInJump * player->GetJumpFlag())));
		}
	}

	for (auto& item : planet)item->Update();//ステージの更新

	userData->dissolveY = player->GetRegenerationRange();//シェーダー用プロパティ

	MyEngine::Physics::GetInstance().Update();//当たり判定の更新

	player->SetMatrix();//行列を反映

}

void SerialPlanetGalaxy::GamePlayingDraw()
{
	if (player->OnAiming())camera->SetDebugCameraPoint();
	Vec3 pos = MV1GetPosition(m_skyDomeH);
	DxLib::MV1DrawModel(m_skyDomeH);

	for (auto& item : planet)
	{
		item->SetIsSearch(player->IsSearch());
		
	}

	MyEngine::Physics::GetInstance().Draw();
	
	if (player->IsSearch())
	{
		DxLib::SetDrawBlendMode(DX_BLENDMODE_MUL, 255);
		// ちょっと暗い矩形を描画
		DxLib::DrawBox(0, 0, 1600, 900,
			0x444488, true);
		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	}
	for (auto& item : booster){item->Draw();}
	
	int alpha = static_cast<int>(255 * (static_cast<float>(player->GetDamageFrame()) / 60.0f));
#ifdef _DEBUG
	Vec3 UIPos = ((Vec3(GetCameraPosition()) + Vec3(GetCameraFrontVector()) * 110) + Vec3(GetCameraLeftVector()) * -70 + Vec3(GetCameraUpVector()) * 37);
	DrawLine3D(UIPos.VGet(), Vec3(UIPos + Vec3::Up() * 20).VGet(), 0xff0000);
	DrawLine3D(UIPos.VGet(), Vec3(UIPos + Vec3::Right() * 20).VGet(), 0x00ff00);
	DrawLine3D(UIPos.VGet(), Vec3(UIPos + Vec3::Front() * 20).VGet(), 0x0000ff);



#endif
	
	DxLib::SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
	DxLib::DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0xff4444, true);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawFormatString(0, 0, 0xffffff, "NormVec(%f,%f,%f)", player->GetNormVec().x, player->GetNormVec().y, player->GetNormVec().z);
	DrawFormatString(0, 25, 0xffffff, "FrontVec(%f,%f,%f)", player->GetFrontVec().x, player->GetFrontVec().y, player->GetFrontVec().z);
	DrawFormatString(0, 50, 0xffffff, "SideVec(%f,%f,%f)", player->GetSideVec().x, player->GetSideVec().y, player->GetSideVec().z);
	DrawFormatString(0, 75, 0xffffff, "shotDir(%f,%f,%f)", player->GetShotDir().x, player->GetShotDir().y, player->GetShotDir().z);
	DrawFormatString(0, 100, 0xffffff, "Camera(%f,%f,%f),Length(%f)",camera->GetPos().x, camera->GetPos().y, camera->GetPos().z,(camera->GetPos() - player->GetPos()).Length());
	
	DrawFormatString(0, 150, 0xffffff, "PlayerPos(%f,%f,%f)", player->GetPos().x, player->GetPos().y, player->GetPos().z);
}
