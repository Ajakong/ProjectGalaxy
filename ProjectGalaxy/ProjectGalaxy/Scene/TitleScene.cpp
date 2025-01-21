#include <DxLib.h>
#include <cassert>
#include "Application.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "GameOverScene.h"
#include"GamePlayingScene.h"
#include"Game.h"
#include"Pad.h"
#include"GraphManager.h"
#include"SoundManager.h"
#include"ModelManager.h"
#include"Player.h"
#include"Physics.h"
#include"SpherePlanet.h"
#include"Camera.h"


namespace
{
	constexpr int kFadeFrameMax = 60;
	constexpr int kStandByFrame = 120;

	// 画面の幅と高さ
	constexpr int screenWidth = 1600;
	constexpr int screenHeight = 900;

	constexpr int kLightningFrameMax = 200;

	const char* kTitleGraphName = "galaxy_titleLogo_pro.png";
	const char* kFrameName = "Frame.png";
	const char* kTitleBGMName = "Title.mp3";
	const char* kGameStartSEName = "StartGame2.mp3";
	const char* kGameBGMName = "GamePlaying.mp3";
	const char* kPlayerFileName = "SpaceHarrier.mv1";

}

TitleScene::TitleScene(SceneManager& manager) :
	Scene(manager),
	m_btnFrame(0),
	m_fadeSpeed(1),
	m_titleHandle(GraphManager::GetInstance().GetGraphData(kTitleGraphName)),
	m_titleBGMHandle(SoundManager::GetInstance().GetSoundData(kTitleBGMName)),
	m_gameStartSEHandle(SoundManager::GetInstance().GetSoundData(kGameStartSEName)),
	player(std::make_shared<Player>(ModelManager::GetInstance().GetModelData(kPlayerFileName))),
	planet(std::make_shared<SpherePlanet>(Vec3(0, -50, 0), 0xaadd33, 3.f,ModelManager::GetInstance().GetModelData("GoldenBall.mv1"), 1.0f, 1))
{
	SetCameraPositionAndTarget_UpVecY(VGet(-200, -45, 80), VGet(0, -45, 80));
	SetCameraNearFar(10.f, 3000.f);

	MyEngine::Physics::GetInstance().Entry(planet);

	MyEngine::Physics::GetInstance().Entry(player);
	m_updateFunc = &TitleScene::NormalUpdate;
	m_drawFunc = &TitleScene::NormalDraw;
	//camera = make_shared<Camera>();

}

TitleScene::~TitleScene()
{
	MyEngine::Physics::GetInstance().Exit(player);
	MyEngine::Physics::GetInstance().Exit(planet);
}

void TitleScene::Load()
{
}

void TitleScene::Update()
{
	/*camera->SetEasingSpeed(player->GetCameraEasingSpeed());
	camera->SetCameraPoint(player->GetPos() + player->GetSideVec().GetNormalized() * 20);*/
	//m_camera->SetCameraPoint(player->GetPos() + Vec3::Left() * 30);
	
	MyEngine::Physics::GetInstance().Update();
	player->SetMatrix();

	(this->*m_updateFunc)();
	Pad::Update();
}

void TitleScene::Draw()
{

	MyEngine::Physics::GetInstance().Draw();

	(this->*m_drawFunc)();
	/*DrawFormatString(0, 25 * 6, 0xffffff, "PlayerPos(%f,%f,%f)", player->GetPos().x, player->GetPos().y, player->GetPos().z);
	DrawFormatString(0, 25 * 7, 0xffffff, player->GetState().c_str());

	DrawFormatString(0, 25 * 9, 0xffffff, "FootNowOnHit:%d", player->GetFootOnHit());
	DrawFormatString(0, 25 * 10, 0xffffff, "PlayerVelocity(%f,%f,%f)", player->GetRigidbody()->GetVelocity().x, player->GetRigidbody()->GetVelocity().y, player->GetRigidbody()->GetVelocity().z);*/
}

void TitleScene::FadeInUpdate()
{
	m_fps = GetFPS();
	MyEngine::Physics::GetInstance().Update();
	m_frame--;
	if (m_frame <= 0)
	{
		m_updateFunc = &TitleScene::NormalUpdate;
		m_drawFunc = &TitleScene::NormalDraw;
	}
}

void TitleScene::NormalUpdate()
{
	m_fps = GetFPS();
	MyEngine::Physics::GetInstance().Update();
	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		m_updateFunc = &TitleScene::FadeOutUpdate;
		m_drawFunc = &TitleScene::FadeDraw;
	}

	m_btnFrame += m_fadeSpeed;
	if (m_btnFrame > kFadeFrameMax)m_fadeSpeed *= -1;
	if (m_btnFrame < 0)m_fadeSpeed *= -1;

}

void TitleScene::FadeOutUpdate()
{
	m_fps = GetFPS();
	MyEngine::Physics::GetInstance().Update();
	m_frame++;
	if (m_frame >= 120) {
		m_isGamePlaying = true;
	}
}

void TitleScene::ChangeScene(std::shared_ptr<Scene> next)
{
	m_manager.ChangeScene(next);
}

void TitleScene::FadeDraw()
{
	int alpha = static_cast<int>(255 * (static_cast<float>(m_frame) / kFadeFrameMax));

	DrawExtendGraph(0, 150,800,600, m_titleHandle, true);
	DrawFormatString(350, 600, 0xffffff, "Push A to Start");

	SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	DrawBox(0, 0, m_frame * 30, 1600, 0x001111, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (m_isGamePlaying)
	{
		ChangeScene(std::make_shared<GamePlayingScene>(m_manager));
	}
	DrawLine(m_frame * 30, 0, m_frame * 30, 900, 0x44ffff);
}

void TitleScene::NormalDraw()
{
	int alpha = static_cast<int>(255 * (static_cast<float>(m_frame) / kFadeFrameMax));

	DrawExtendGraph(0, 150, 800, 600, m_titleHandle, true);
	DrawFormatString(350, 600, 0xffffff, "Push A to Start");

	SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	DrawBox(0, 0, m_frame * 30, 1600, 0x001111, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (m_isGamePlaying)
	{
		ChangeScene(std::make_shared<GamePlayingScene>(m_manager));
	}
	DrawLine(m_frame * 30, 0, m_frame * 30, 900, 0x44ffff);
}
