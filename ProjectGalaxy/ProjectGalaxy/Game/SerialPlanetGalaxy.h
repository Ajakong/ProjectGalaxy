#pragma once
#include"Galaxy.h"

class Camera;
class Player;
class Planet;
class BossPlanet;
class ClearObject;
class WarpGate;
class Takobo;
class KillerTheSeeker;
class Gorori;
class Item;

class SerialPlanetGalaxy :public Galaxy//GameManager���Ǘ�,�f����I�u�W�F�N�g�̏��������Ă���
{
public:
	SerialPlanetGalaxy(std::shared_ptr<Player> playerPointer);
	virtual ~SerialPlanetGalaxy();

	void Init();
	void Update();
	void Draw();
	int GetMaterialXCount() { return static_cast<int>(11 - poworStone.size()); }

	using managerState_t = void(SerialPlanetGalaxy::*)();
	managerState_t m_managerUpdate;

	using managerState_t = void(SerialPlanetGalaxy::*)();
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

	std::shared_ptr<Camera> camera;
	std::vector<std::shared_ptr<Planet>> planet;
	std::shared_ptr<BossPlanet> bossPlanet;
	std::vector<std::shared_ptr<ClearObject>> clearObject;
	std::vector<std::shared_ptr<WarpGate>> warpGate;
	std::vector<std::shared_ptr<Takobo>> takobo;
	std::vector<std::shared_ptr<KillerTheSeeker>> killerTheSeeker;
	std::vector<std::shared_ptr<Gorori>> gorori;
	std::vector<std::shared_ptr<Item>> poworStone;

	Vec3 m_cameraUpVec;

	//�{�X���o��������
	bool m_isBossWatch = false;

	//�n���h���W
	int m_skyDomeH;
	int modelH;
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

