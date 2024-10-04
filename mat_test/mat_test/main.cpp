#include "DxLib.h"


namespace
{
    constexpr int kNetralRadius = 20;//�ʏ펞�̓����蔼�a

    constexpr int kDamageFrameMax = 20;
    //�A�j���[�V�����ԍ�
    constexpr int kIdleAnimIndex = 1;
    //constexpr int kIdleAnimIndex = 2;//�ҋ@�e�X�g
    constexpr int kAttackAnimIndex = 30;

    constexpr float kAnimFrameSpeed = 30.0f;//�A�j���[�V�����i�s���x

    //�A�j���[�V�����̐؂�ւ��ɂ�����t���[����
    constexpr float kAnimChangeFrame = 8.0f;
    constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;

    //�A�i���O�X�e�B�b�N�ɂ��ړ��֘A
    constexpr float kMaxSpeed = 5.0f;//�v���C���[�̍ő呬�x
    constexpr float kAnalogRangeMin = 0.1f;//�A�i���O�X�e�B�b�N�̓��͔���͈�
    constexpr float kAnalogRangeMax = 0.8f;
    constexpr float kAnalogInputMax = 1000.0f;//�A�i���O�X�e�B�b�N������͂����x�N�g���̍ő�l

    constexpr float kFrameParSecond = 60.0f;

    constexpr int kAvoidFrame = 60;

    constexpr int kJumpPower = 50;

    constexpr int kSearchRemainTimeMax = 28;
    constexpr int kChargeRemainCoolTime = 10;

    const char* kGororiHitSEName = "GororiHitSE.mp3";
    const char* kGetItemSEName = "GetItemSE.mp3";
    const char* kOnParrySEName = "Parry.mp3";

    const char* kGetSearchSEName = "Search.mp3";
    const char* name = "Player";
    const char* kFileName = "SpaceHarrier.mv1";

    constexpr int kAnimationNumTpose = 0;
    constexpr int kAnimationNumHit = 1;
    constexpr int kAnimationNumJump = 2;
    constexpr int kAnimationNumRun = 3;
    constexpr int kAnimationNumSpin = 4;
    constexpr int kAnimationNumIdle = 5;

    //�Ə�
    const char* kAimGraphFileName = "Elements_pro.png";
    constexpr int kAimGraphSrkX = 3140;
    constexpr int kAimGraphSrkY = 200;
    constexpr int kAimGraphWidth = 400;
    constexpr int kAimGraphHeight = 370;
}


bool UpdateAnim(int attachNo,int modelHandle)
{
    //�A�j���[�V�������ݒ肳��Ă��Ȃ��̂ŏI��
    if (attachNo == -1) return false;

    //�A�j���[�V������i�s������
    float now = MV1GetAttachAnimTime(modelHandle, attachNo);//���݂̍Đ��J�E���g
    now += kAnimFrameSpeed / kFrameParSecond;//�A�j���[�V�����J�E���g��i�߂�


    //���ݍĐ����̃A�j���[�V�����̑��J�E���g���擾����
    float total = MV1GetAttachAnimTotalTime(modelHandle, attachNo);
    bool isLoop = false;
    while (now >= total)
    {
        now -= total;
        isLoop = true;
    }

    MV1SetAttachAnimTime(modelHandle, attachNo, now);

    return isLoop;
}

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) 
{
    ChangeWindowMode(true); // �E�B���h�E���[�h�ɂ��܂�
    //SetGraphMode(m_windowSize.w, m_windowSize.h, 1);

    SetUseDirect3DVersion(DX_DIRECT3D_11);


    SetWindowSizeChangeEnableFlag(TRUE, TRUE);
    SetChangeScreenModeGraphicsSystemResetFlag(false);

    SetWindowSize(1600, 900);

    void* CallBack();


    DxLib_Init();

    SetUseASyncChangeWindowModeFunction(TRUE, NULL, NULL);

    SetUseZBuffer3D(true);
    SetWriteZBuffer3D(true);
    SetUseBackCulling(true);


    SetWindowText("�Ȃ߂������߂Ă�������");
    if (DxLib_Init() == -1)
    {
        return false;
    }

    SetDrawScreen(DX_SCREEN_BACK);

    LONGLONG time;

    int modelHandle = MV1LoadModel("SpaceHarrier.mv1");
    int prevAnimNo=-1;
    int currentAnimNo=0;
    int animBlendRate=0;
    // FOV(����p)��60�x��
    SetupCamera_Perspective(60.0f * (static_cast<float>(DX_PI_F) / 180.0f));

    CreateSpotLightHandle(VGet(0, 0, 0), VGet(0, -1, 0), DX_PI_F * 2,
        DX_PI_F * 2,
        2000.0f,
        0.0f,
        0.002f,
        0.0f);

    SetGlobalAmbientLight(GetColorF(0.0f, 0.0f, 1.0f, 1.0f));
    MV1AttachAnim(modelHandle, 0);
    float rotate = 0;
    VECTOR nowVec= VGet(0, 0, -1);
    VECTOR dir;
    auto ChangeAnim = [&](int animIndex) { //����ɌÂ��A�j���[�V�������A�^�b�`����Ă���ꍇ�͂��̎��_�ō폜���Ă���
        if (prevAnimNo != -1)
        {
            MV1DetachAnim(modelHandle, prevAnimNo);
        }

        //���ݍĐ����̑ҋ@�A�j���[�V�����͕ύX�O�̃A�j���[�V����������
        prevAnimNo = currentAnimNo;

        //�ύX��̃A�j���[�V�����Ƃ��čU���A�j���[�V���������߂Đݒ肷��
        currentAnimNo = MV1AttachAnim(modelHandle, animIndex, -1, false);

        //�؂�ւ��̏u�Ԃ͕ύX�O�̃A�j���[�V�������Đ�������Ԃɂ���
        animBlendRate = 0.0f;

        //�ύX�O�̃A�j���[�V����100%
        MV1SetAttachAnimBlendRate(modelHandle, prevAnimNo, 1.0f - animBlendRate);
        //�ύX��̃A�j���[�V����0%
        MV1SetAttachAnimBlendRate(modelHandle, currentAnimNo, animBlendRate); };


    ChangeAnim(kAnimationNumIdle);
   
    while (ProcessMessage() != -1)
    {
        // FPS�̌Œ�悤�ɊJ�n���̎��Ԃ��擾
        time = GetNowHiPerformanceCount();

        ClearDrawScreen();
        if (CheckHitKey(KEY_INPUT_ESCAPE))
        {
            ChangeWindowMode(true);
        }
        SetCameraPositionAndTargetAndUpVec(VGet(0,0,-200),VGet(0,0,0),VGet(0,1,0));

        int directX = 0, directY = 0;
        GetJoypadAnalogInputRight(&directX, &directY, DX_INPUT_PAD1);
        directY = -directY;

        

        dir = VScale(VGet(1,0,0) , static_cast<float>(directX) * 0.001f);
        dir=VAdd(dir , VScale( VGet(0,1,0), static_cast<float>(directY) * 0.001f));
        dir=VAdd(dir,VGet(0,0,-1));
        dir=VNorm(dir);


        MV1SetScale(modelHandle,VGet(0.05f, 0.05f, 0.05f));
        MV1SetPosition(modelHandle, VGet(0, -30, 0));

        //rotate += 0.1f;
        int index = MV1SearchFrame(modelHandle, "mixamorig:Spine");
        MATRIX shotDirMat = MGetRotVec2(nowVec,dir);
        nowVec = dir;

        MATRIX localMat = MV1GetFrameLocalMatrix(modelHandle, index);
        MATRIX mat = MMult(shotDirMat, localMat);
        MV1SetFrameUserLocalMatrix(modelHandle, index, mat);
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0;j < 4; j++)
            {
                DrawFormatString(i * 80, j * 16, GetColor(255, 255, 255),"%f", mat.m[i][j]);
            }
        }

        if (GetJoypadInputState(DX_INPUT_KEY_PAD1) == PAD_INPUT_3)
        {
            nowVec = VGet(0, 0, -1);
            MV1SetFrameUserLocalMatrix(modelHandle, index, MGetIdent());

        }

        UpdateAnim(modelHandle);
        
        MV1DrawModel(modelHandle);


  
        ScreenFlip();

        // 60FPS�ɌŒ�
        while (16667 > GetNowHiPerformanceCount() - time) {};
    }

    DxLib_End();
	return 0;
}