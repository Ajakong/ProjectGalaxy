#pragma once
#include"MyLib/Vec3.h"

class Galaxy//GameManager���Ǘ�,�f����I�u�W�F�N�g�̏��������Ă���
{
public:
	Galaxy();
	virtual ~Galaxy();

	void Init();
	void Update();
	void Draw();
	int GetMaterialXCount() { return static_cast<int>(11 - poworStone.size()); }

	bool GetGameOver() const { return m_isGameOverFlag; }
	bool GetClear() const { return m_isClearFlag; }

	using managerState_t = void(Galaxy::*)();
	managerState_t m_managerUpdate;

	using managerState_t = void(Galaxy::*)();
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
	int cbuffH = CreateShaderConstantBuffer(sizeof(UserData));
	UserData* userData = static_cast<UserData*>(GetBufferShaderConstantBuffer(cbuffH));

	std::shared_ptr<Ui> ui;
	std::shared_ptr<Camera> camera;
	std::shared_ptr<Player> player;
	std::vector<std::shared_ptr<Planet>> planet;
	std::shared_ptr<BossPlanet> bossPlanet;
	std::vector<std::shared_ptr<ClearObject>> clearObject;
	std::vector<std::shared_ptr<WarpGate>> warpGate;
	std::vector<std::shared_ptr<Takobo>> takobo;
	std::vector<std::shared_ptr<KillerTheSeeker>> killerTheSeeker;
	std::vector<std::shared_ptr<Gorori>> gorori;
	std::vector<std::shared_ptr<Item>> poworStone;

	Vec3 m_cameraUpVec;

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

