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
		float dissolveY;	// ディゾルヴしたい高さ
		float minY;
		float maxY;
		float dummy;
		float clickedU;
		float clickedV;
		float dummy2[2];
	};

	//このフラグを見てシーンを移行する
	bool m_isGameOverFlag = false;
	bool m_isClearFlag = false;

	//ボスが出現したか
	bool m_isBossWatch = false;

	//ハンドル集
	int m_skyDomeH;
	int modelH;
	int textureUIHandle;
	int fontHandle;
	int m_warpEffectHandle;
	int m_miniMapScreenHandle;


	//物体Xの数
	int itemNum;

	// 通常のRT
	int RT;
	int RT2;
	// アウトラインのRT
	int outlineRT;
	// 法線情報のRT
	int normRT;
	// ぼかし用RT
	int blurRT;
	int shrinkRT;
	int depthRT;
};

