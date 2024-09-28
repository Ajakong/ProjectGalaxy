#include "Application.h"
#include "SceneManager.h"
#include "GamePlayingScene.h"
#include"TitleScene.h"
#include"GameOverScene.h"
#include<iostream>
#include<vector>
#include"Game.h"
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include <cassert>

namespace
{
    constexpr int kScreenWidth = 1600;
    constexpr int kScreenHeight = 900;

    constexpr int kColorbitDepth = 32;
    constexpr int kIconID = 573;
}

int MyLoadGraph(const TCHAR* path)
{
    int handle = LoadGraph(path);
    assert(handle != -1);
    return handle;
}

Application::Application()
{
    m_windowSize = Size{ kScreenWidth, kScreenHeight };
}

void Application::Terminate()
{
    Effkseer_End();
    DxLib_End();
}

int Application::GetScreenHandle()
{
    return m_screenHandle;
}

float Application::GetGravity() const
{
    return 0.5f;
}

bool Application::Init()
{
    ChangeWindowMode(true); // ウィンドウモードにします
    //SetGraphMode(m_windowSize.w, m_windowSize.h, 1);

    SetUseDirect3DVersion(DX_DIRECT3D_11);


    SetWindowSizeChangeEnableFlag(TRUE, TRUE);
    SetChangeScreenModeGraphicsSystemResetFlag(false);


    SetGraphMode(kScreenWidth, kScreenHeight, kColorbitDepth);
    SetWindowSize(kScreenWidth, kScreenHeight);

    void* CallBack();


    DxLib_Init();

    SetWindowIconID(kIconID);

    SetUseASyncChangeWindowModeFunction(TRUE, NULL, NULL);

    SetUseZBuffer3D(true);
    SetWriteZBuffer3D(true);
    SetUseBackCulling(true);

    Effekseer_Init(8000);
    Effekseer_InitDistortion();
    Effekseer_SetGraphicsDeviceLostCallbackFunctions();

    SetWindowText("なめぇを決めてください");
    if (DxLib_Init() == -1)
    {
        return false;
    }

    SetDrawScreen(DX_SCREEN_BACK);
    return true;
}

void Application::Run()
{
    {// スコープを強制的に作っている

        SceneManager sceneManager;
        sceneManager.ChangeScene(std::make_shared<GamePlayingScene>(sceneManager));

        m_screenHandle = MakeScreen(Game::kScreenWidth, Game::kScreenHeight, true);

        LONGLONG time;

        while (ProcessMessage() != -1)
        {
            // FPSの固定ように開始時の時間を取得
            time = GetNowHiPerformanceCount();

            ClearDrawScreen();
            if (CheckHitKey(KEY_INPUT_ESCAPE))
            {
                ChangeWindowMode(true);
            }

            Effekseer_Sync3DSetting();

            sceneManager.Update();
            UpdateEffekseer3D();
            sceneManager.Draw();
            DrawEffekseer3D();

            ScreenFlip();

            // 60FPSに固定
            while (16667 > GetNowHiPerformanceCount() - time) {};
        }
    }
    Terminate();
}


const Size& Application::GetWindowSize() const
{
    return m_windowSize;
}