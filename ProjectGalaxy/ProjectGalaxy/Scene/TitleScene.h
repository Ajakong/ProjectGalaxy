﻿#pragma once
#include "Scene.h"
#include"Vec3.h"
#include <string>
#include <vector>
#include <map>

class SceneMain;
class TitlePlayer;
class SpherePlanet;
class Camera;


class TitleScene : public Scene
{
public:
    TitleScene(SceneManager& manager);
    ~TitleScene();
    void Load();
    void Update();
    void Draw();

    void getSlectNumber(int a) { m_selectNum = a; }

private:

    int m_btnFrame = 0;
    int m_fadeSpeed = 0;
    int m_frame = 0;

    float m_fps = 0.0f;

    int m_selectNum = 0;

    bool m_isGamePlaying = false;

    int m_titleHandle = 0;
    int m_titleBGMHandle;
    int m_gameStartSEHandle;
    int m_modelHandle[10] = { 0 };
    int m_model_dance = 0;
    int m_attach_move[10] = { 0 };
    int m_gamePlayingBgmHandle;
    int m_frameHandle = 0;

	int m_stickOSTHandle = 0;

    int m_count;



    //ハンドル集
    int m_skyDomeH;
    float m_skyDomeRotationAngle;

    std::shared_ptr<TitlePlayer> player;
	std::shared_ptr<SpherePlanet> planet;
    std::shared_ptr<SpherePlanet> nextPlanet;
    std::shared_ptr<SpherePlanet> emeraldPlanet;
	std::shared_ptr<Camera> camera;

    Vec3 positioningPlayerToCamera;

    // メンバ関数ポインタの宣言
    using UpdateFunc_t = void (TitleScene::*)();
    using DrawFunc_t = void (TitleScene::*)();
    UpdateFunc_t m_updateFunc = nullptr;
    DrawFunc_t m_drawFunc = nullptr;



    // 更新関数
    void FadeInUpdate();
    void NormalUpdate();
    void FadeOutUpdate();
    void DirectionUpdate();
    void LoadingUpdate();

    void ChangeScene(std::shared_ptr<Scene> next);

    // 描画関数
    void FadeDraw();
    void NormalDraw();

};



