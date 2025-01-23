#include <DxLib.h>
#include <cassert>
#include "Application.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "GameOverScene.h"
#include "GamePlayingScene.h"
#include "Game.h"
#include "Pad.h"
#include "GraphManager.h"
#include "SoundManager.h"
#include "ModelManager.h"
#include "TitlePlayer.h"
#include "Physics.h"
#include "SpherePlanet.h"
#include "Camera.h"

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

    const char* kStickName = "parry.mp3";
    const char* kGameStartSEName = "StartGame2.mp3";
    const char* kGameBGMName = "GamePlaying.mp3";
    const char* kPlayerModelName = "SpaceHarrier";
    const char* kPlanetModelName = "GoldenBall";
    const char* kNextPlanetModelName = "Moon";
	const char* kEmeraldPlanetModelName = "GreenMoon";
    const char* kSkyboxModelName = "Skybox";

    constexpr float kCameraNear = 10.0f;
    constexpr float kCameraFar = 3000.0f;
    constexpr float kSkyDomeScale = 0.3f;
    constexpr float kSkyDomeRotationSpeed = 0.001f;

    constexpr int kTitleGraphX = 0;
    constexpr int kTitleGraphY = 150;
    constexpr int kTitleGraphWidth = 800;
    constexpr int kTitleGraphHeight = 600;
    constexpr int kTitleTextX = 350;
    constexpr int kTitleTextY = 600;
    constexpr int kFadeBoxWidth = 1600;
    constexpr int kFadeBoxHeight = 900;
    constexpr int kFadeBoxColor = 0x001111;
    constexpr int kLineColor = 0x44ffff;
    constexpr int kLineX = 30;
    constexpr int kLineY = 900;
}

TitleScene::TitleScene(SceneManager& manager) :
    Scene(manager),
    m_titleBGMHandle(SoundManager::GetInstance().GetSoundData(kTitleBGMName)),
    m_gameStartSEHandle(SoundManager::GetInstance().GetSoundData(kGameStartSEName)),
    m_btnFrame(0),
    m_fadeSpeed(1),
    m_titleHandle(GraphManager::GetInstance().GetGraphData(kTitleGraphName)),
    player(std::make_shared<TitlePlayer>(ModelManager::GetInstance().GetModelData(kPlayerModelName))),
    planet(std::make_shared<SpherePlanet>(Vec3(0, -50, 0), 0xaadd33, 3.f, ModelManager::GetInstance().GetModelData(kPlanetModelName), 1.0f, 1)),
    nextPlanet(std::make_shared<SpherePlanet>(Vec3(-300, -50, 200), 0xaadd33, 3.f, ModelManager::GetInstance().GetModelData(kNextPlanetModelName), 1.0f, 1)),
	emeraldPlanet(std::make_shared<SpherePlanet>(Vec3(200, -50, 200), 0xaadd33, 3.f, ModelManager::GetInstance().GetModelData(kEmeraldPlanetModelName), 1.0f, 1)),
	camera(std::make_shared<Camera>(VGet(-200, -45, 80))),
    m_skyDomeH(0),
    m_skyDomeRotationAngle(0),
	m_count(0)
{
    
    camera->Update(VGet(0, -45, 80));

    PlaySoundMem(m_titleBGMHandle, DX_PLAYTYPE_LOOP);
    //SetCameraPositionAndTarget_UpVecY(VGet(-200, -45, 80), VGet(0, -45, 80));
    //SetCameraNearFar(kCameraNear, kCameraFar);

    MyEngine::Physics::GetInstance().Entry(planet);
    MyEngine::Physics::GetInstance().Entry(nextPlanet);
    MyEngine::Physics::GetInstance().Entry(emeraldPlanet);
    MyEngine::Physics::GetInstance().Entry(player);

    m_updateFunc = &TitleScene::NormalUpdate;
    m_drawFunc = &TitleScene::NormalDraw;

    m_skyDomeH = ModelManager::GetInstance().GetModelData(kSkyboxModelName);
    MV1SetPosition(m_skyDomeH, VGet(0, 0, 0));
    MV1SetScale(m_skyDomeH, VGet(kSkyDomeScale, kSkyDomeScale, kSkyDomeScale));
}

TitleScene::~TitleScene()
{
    MyEngine::Physics::GetInstance().Exit(player);
    MyEngine::Physics::GetInstance().Exit(planet);
    MyEngine::Physics::GetInstance().Exit(nextPlanet);

}

void TitleScene::Load()
{
}

void TitleScene::Update()
{
   

    planet->ModelRotation();
	nextPlanet->ModelRotation();
	emeraldPlanet->ModelRotation();
    MyEngine::Physics::GetInstance().Update();
    player->SetMatrix();
    (this->*m_updateFunc)();
    m_skyDomeRotationAngle += kSkyDomeRotationSpeed;
    MV1SetRotationXYZ(m_skyDomeH, VGet(0, m_skyDomeRotationAngle, 0));

    Pad::Update();
}

void TitleScene::Draw()
{
    // 画面とZバッファのクリア
    ClearDrawScreen();
   
    // Zバッファの有効化
    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);

    // スカイドームの描画
    DxLib::MV1DrawModel(m_skyDomeH);
    // 物理エンジンの描画
    MyEngine::Physics::GetInstance().Draw();
    // シーン固有の描画
    (this->*m_drawFunc)();

   

    // Zバッファの無効化
    SetUseZBuffer3D(FALSE);
    SetWriteZBuffer3D(FALSE);

    //// 2D描画
    //DrawFormatString(0, 25 * 6, 0xffffff, "PlayerPos(%f,%f,%f)", player->GetPos().x, player->GetPos().y, player->GetPos().z);
    //DrawFormatString(0, 25 * 7, 0xffffff, player->GetState().c_str());
    //DrawFormatString(0, 25 * 9, 0xffffff, "FootNowOnHit:%d", player->GetFootOnHit());
    //DrawFormatString(0, 25 * 10, 0xffffff, "PlayerVelocity(%f,%f,%f)", player->GetRigidbody()->GetVelocity().x, player->GetRigidbody()->GetVelocity().y, player->GetRigidbody()->GetVelocity().z);
}

void TitleScene::FadeInUpdate()
{
    m_fps = GetFPS();
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

   
   
    player->SetMatrix();
    if (Pad::IsTrigger(PAD_INPUT_1))
    {
        player->MoveToTargetPosWithSticker(nextPlanet->GetRigidbody()->GetPos());
        m_updateFunc = &TitleScene::DirectionUpdate;
    }

   
    

    m_btnFrame += m_fadeSpeed;
    if (m_btnFrame > kFadeFrameMax)m_fadeSpeed *= -1;
    if (m_btnFrame < 0)m_fadeSpeed *= -1;
}

void TitleScene::FadeOutUpdate()
{
    camera->Update(player->GetRigidbody()->GetPos());
    camera->SetCameraPoint(player->GetPos() + positioningPlayerToCamera * 10);
    player->MoveToTargetWithStickStar(Vec3(0, 0, 0));
    m_fps = GetFPS();
    m_frame++;
   
    if (m_frame >= kStandByFrame) {
        m_isGamePlaying = true;
    }
}

void TitleScene::DirectionUpdate()
{
   
    bool moveFlag = player->MoveToTargetPosWithSticker(nextPlanet->GetRigidbody()->GetPos());
    if (moveFlag)
    {
        m_updateFunc = &TitleScene::LoadingUpdate;
        //m_drawFunc = &TitleScene::FadeDraw;
    }
}

void TitleScene::LoadingUpdate()
{
    m_count++;
    camera->Update(player->GetRigidbody()->GetPos());

    if (m_count == 70)
    {
        positioningPlayerToCamera = camera->GetPos() - player->GetPos();
        positioningPlayerToCamera.Normalize();
    }

    if (m_count > 70)
    {
        camera->SetEasingSpeed(15.f);
        camera->SetCameraPoint(player->GetPos() + positioningPlayerToCamera * 10);
        player->DoNotMove();
        m_updateFunc = &TitleScene::FadeOutUpdate;
        m_drawFunc = &TitleScene::FadeDraw;
    }
    
}

void TitleScene::ChangeScene(std::shared_ptr<Scene> next)
{
    StopSoundMem(m_titleBGMHandle);
    m_manager.ChangeScene(next);
}

void TitleScene::FadeDraw()
{
    int alpha = static_cast<int>(255 * (static_cast<float>(m_frame) / kFadeFrameMax));

    DrawExtendGraph(kTitleGraphX, kTitleGraphY, kTitleGraphWidth, kTitleGraphHeight, m_titleHandle, true);
    DrawFormatString(kTitleTextX, kTitleTextY, 0xffffff, "Push A to Start");

    SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
    DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
    DrawBox(0, 0, m_frame * kLineX, kFadeBoxWidth, kFadeBoxColor, true);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    if (m_isGamePlaying)
    {
        ChangeScene(std::make_shared<GamePlayingScene>(m_manager));
    }
    DrawLine(m_frame * kLineX, 0, m_frame * kLineX, kLineY, kLineColor);
}

void TitleScene::NormalDraw()
{
    int alpha = static_cast<int>(255 * (static_cast<float>(m_frame) / kFadeFrameMax));

    DrawExtendGraph(kTitleGraphX, kTitleGraphY, kTitleGraphWidth, kTitleGraphHeight, m_titleHandle, true);
    int btnalpha = static_cast<int>(255 * (static_cast<float>(m_btnFrame) / kFadeFrameMax));
    SetDrawBlendMode(DX_BLENDMODE_ADD, btnalpha);

    DrawFormatString(kTitleTextX, kTitleTextY, 0xffffff, "Push A to Start");
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
    DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
    DrawBox(0, 0, m_frame * kLineX, kFadeBoxWidth, kFadeBoxColor, true);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    if (m_isGamePlaying)
    {
        ChangeScene(std::make_shared<GamePlayingScene>(m_manager));
    }
    DrawLine(m_frame * kLineX, 0, m_frame * kLineX, kLineY, kLineColor);
}