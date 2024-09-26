#pragma once
#include"MyLib/Vec3.h"

class Galaxy//GameManagerが管理,惑星やオブジェクトの情報を持っている
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

