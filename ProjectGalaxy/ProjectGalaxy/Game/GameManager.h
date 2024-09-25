#pragma once
#include"MyLib/Vec3.h"

class Vec3;
class Ui;
class Camera;
class Player;
class Planet;
class BossPlanet;
class Takobo;
class KillerTheSeeker;
class Gorori;
class Item;
class ClearObject;
class WarpGate;

class GameManager
{
public:
	GameManager();
	~GameManager();
	void Init();
	void Update();
	void Draw();

	bool GetGameOver() const { return m_isGameOverFlag; }
	bool GetClear() const { return m_isClearFlag; }

	using managerState_t = void(GameManager::*)();
	managerState_t m_managerUpdate;

	using managerState_t = void(GameManager::*)();
	managerState_t m_managerDraw;
private:
	void IntroUpdate();
	void IntroDraw();

	void GamePlayingUpdate();
	void GamePlayingDraw();

private:

	struct UserData
	{
		float dissolveY;	// �f�B�]��������������
		float minY;
		float maxY;
		float dummy;
		float clickedU;
		float clickedV;
		float dummy2[2];
	};

	//���̃t���O�����ăV�[�����ڍs����
	bool m_isGameOverFlag = false;
	bool m_isClearFlag = false;

	//�{�X���o��������
	bool m_isBossWatch = false;

	//�n���h���W
	int m_skyDomeH;
	int modelH;
	int textureUIHandle;
	int fontHandle;
	int m_warpEffectHandle;
	int m_miniMapScreenHandle;


	//����X�̐�
	int itemNum;

	// �ʏ��RT
	int RT;
	int RT2;
	// �A�E�g���C����RT
	int outlineRT;
	// �@������RT
	int normRT;
	// �ڂ����pRT
	int blurRT;
	int shrinkRT;
	int depthRT;
};

