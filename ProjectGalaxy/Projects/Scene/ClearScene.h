﻿#pragma once
#include "Scene.h"

class ClearScene : public Scene
{
public:
    ClearScene(SceneManager& mgr);
    ~ClearScene();

    void Load();
    void Update();
    void Draw();

private:

    // 更新状態を表す関数
    void FadeInUpdate();
    void NormalUpdate();
    void FadeOutUpdate();

    void ChangeScene(std::shared_ptr<Scene> nextScene);

    // 描画状態を表す関数
    void FadeDraw();
    void NormalDraw();

private:

    int m_btnFrame;
    int m_frame;
    int m_numFontHandle;
    int m_fontHandle;

    int m_themeHandle;

    // メンバ関数ポインタの宣言
    using UpdateFunc_t = void (ClearScene::*)();
    using DrawFunc_t = void (ClearScene::*)();
    UpdateFunc_t m_updateFunc;
    DrawFunc_t m_drawFunc;

};

