﻿#pragma once
#include"Galaxy.h"

class Camera;
class Player;
class Planet;
class BossPlanet;
class ClearObject;
class WarpGate;

//ギミック
class Booster;
class StarCapture;

class Takobo;
class KillerTheSeeker;
class Gorori;
class Item;

class SerialPlanetGalaxy :public Galaxy//GameManagerが管理,惑星やオブジェクトの情報を持っている
{
public:
	SerialPlanetGalaxy(std::shared_ptr<Player> playerPointer);
	virtual ~SerialPlanetGalaxy();

	void Init();
	void Update();
	void Draw();
	
	using managerState_t = void(SerialPlanetGalaxy::*)();
	managerState_t m_managerUpdate;

	using managerState_t = void(SerialPlanetGalaxy::*)();
	managerState_t m_managerDraw;
private:
	/// <summary>
	/// ステージ紹介の更新処理
	/// </summary>
	void IntroUpdate();
	/// <summary>
	/// ステージ紹介の描画処理
	/// </summary>
	void IntroDraw();

	/// <summary>
	/// ゲームプレイ時の更新処理
	/// </summary>
	void GamePlayingUpdate();
	/// <summary>
	/// ゲームプレイ時の描画処理
	/// </summary>
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
	int cbuffH = CreateShaderConstantBuffer(sizeof(UserData));
	UserData* userData = static_cast<UserData*>(GetBufferShaderConstantBuffer(cbuffH));

	std::shared_ptr<Camera> camera;
	std::vector<std::shared_ptr<Planet>> planet;
	std::shared_ptr<BossPlanet> bossPlanet;
	std::vector<std::shared_ptr<ClearObject>> clearObject;
	std::vector<std::shared_ptr<WarpGate>> warpGate;
	std::vector<std::shared_ptr<Booster>> booster;
	std::vector<std::shared_ptr<StarCapture>> starCapture;
	std::vector<std::shared_ptr<Takobo>> takobo;
	std::vector<std::shared_ptr<KillerTheSeeker>> killerTheSeeker;
	std::vector<std::shared_ptr<Gorori>> gorori;
	std::vector<std::shared_ptr<Item>> poworStone;

	Vec3 m_cameraUpVec;

	//ボスが出現したか
	bool m_isBossWatch = false;

	//ハンドル集
	int m_skyDomeH;
	int modelH;
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
