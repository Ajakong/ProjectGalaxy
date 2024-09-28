#include"DxLib.h"
#include"Galaxy.h"
// EffekseerForDXLib.hをインクルードします。
#include "EffekseerForDXLib.h"
#include"Camera.h"
#include"Physics.h"
#include"Player.h"
#include"WarpGate.h"
#include"BossPlanet.h"
#include"Takobo.h"
#include"KillerTheSeeker.h"
#include"Gorori.h"
#include"Item.h"
#include"ClearObject.h"
#include<cassert>
#include"Pad.h"
#include"SoundManager.h"
#include"GraphManager.h"
#include"FontManager.h"
#include"ScreenManager.h"
#include"ModelManager.h"
#include"Game.h"

using namespace std;

namespace
{
	//UI
	//ミッション
	constexpr int kUiText_SrkX = 150;
	constexpr int kUiText_SrkY = 170;
	constexpr int kUiText_Width = 1050;
	constexpr int kUiText_Height = 450;
	constexpr float kUiText_Exrate = 0.2f;

	//HPバーの枠
	constexpr int kUiHpbarFrame_PosX = 150;
	constexpr int kUiHpbarFrame_PosY = 35;
	constexpr int kUiHpbarFrame_SrkX = 2600;
	constexpr int kUiHpbarFrame_SrkY = 2560;
	constexpr int kUiHpbarFrame_Width = 1000;
	constexpr int kUiHpbarFrame_Height = 144;
	constexpr float kUiHpbarFrame_Exrate = 0.3f;

	//ChargeRemainTimeのバー
	constexpr int kUiCRT_PosX = 20;
	constexpr int kUiCRT_PosY = 70;
	constexpr int kUiCRT_SrkX = 2655;
	constexpr int kUiCRT_SrkY = 2720;
	constexpr int kUiCRT_Width = 30;
	constexpr int kUiCRT_DisX = 9;
	constexpr int kUiCRT_Height = 80;
	constexpr float kUiCRT_Exrate = 0.3f;

	//HPバー
	constexpr int kUiHpbar_PosX = 15;
	constexpr int kUiHpbar_PosY = 25;
	constexpr int kUiHpbar_Height = 23;
	constexpr float kUiHpbar_mag = 5.35f;//HPに掛けたらいい感じになる倍率

	//タイマーの枠
	constexpr int kUiTimeCountFrame_PosX = 1400;
	constexpr int kUiTimeCountFrame_PosY = 100;
	constexpr int kUiTimeCountFrame_SrkX = 1280;
	constexpr int kUiTimeCountFrame_SrkY = 130;
	constexpr int kUiTimeCountFrame_Width = 1040;
	constexpr int kUiTimeCountFrame_Height = 410;
	constexpr float kUiTimeCountFrame_Exrate = 0.3f;

	//タイマー
	constexpr int kUiTimeCount_PosX = 1350;
	constexpr int kUiTimeCount_PosY = 90;

	//カメラ
	constexpr float kCameraDistanceFront = 800.f;
	constexpr float kCameraDistanceAddFrontInJump = 300.f;
	constexpr float kCameraDistanceUp = 500.f;


	const char* kMiniMapScreenName = "MiniMap";
}

Galaxy::Galaxy() :
	// 通常のRT
	RT(MakeScreen(640, 480, true)),
	RT2(MakeScreen(640, 480, true)),
	// アウトラインのRT
	outlineRT(MakeScreen(640, 480, true)),
	// 法線情報のRT
	normRT(MakeScreen(640, 480)),
	// ぼかし用RT
	blurRT(MakeScreen(640, 480, true)),
	shrinkRT(MakeScreen(320, 240, true)),
	depthRT(MakeScreen(640, 480)),
	m_isClearFlag(false),
	itemNum(0),
	m_isBossWatch(false)
{
	player = std::make_shared<Player>(modelH);
	camera = std::make_shared<Camera>();
	planet.push_back(std::make_shared<SpherePlanet>(Vec3(0, -500, 0), 0xaadd33, 3, ModelManager::GetInstance().GetModelData("Sphere/planet_moon.mv1")));
	planet.push_back(std::make_shared<SpherePlanet>(Vec3(6000, 0, 2000), 0x4444ff, 3, ModelManager::GetInstance().GetModelData("Sphere/planet_ice.mv1")));
	planet.push_back(std::make_shared<SpherePlanet>(Vec3(-3000, 1000, -3000), 0xff4400, 1.f, ModelManager::GetInstance().GetModelData("Sphere/planet_red.mv1")));
	bossPlanet = std::make_shared<BossPlanet>(Vec3(0, -6000, 0), 0x0000ff);
	takobo = { std::make_shared<Takobo>(Vec3(1000,0,500)),std::make_shared<Takobo>(Vec3(-300,0,500)),std::make_shared<Takobo>(Vec3(0,900,500)) };
	gorori = { std::make_shared<Gorori>(Vec3(7000,500,2300)),std::make_shared<Gorori>(Vec3(6500,500,1700)),std::make_shared<Gorori>(Vec3(5500,0,2000)) };
	poworStone.push_back(std::make_shared<Item>(Vec3(0, -800, 0), true));
	poworStone.push_back(std::make_shared<Item>(Vec3(-300, 0, 0), true));
	poworStone.push_back(std::make_shared<Item>(Vec3(300, 0, 0), true));
	poworStone.push_back(std::make_shared<Item>(Vec3(-200, -800, 0), true));
	poworStone.push_back(std::make_shared<Item>(Vec3(0, 0, 500), true));
	poworStone.push_back(std::make_shared<Item>(Vec3(0, 0, -500), true));
	poworStone.push_back(std::make_shared<Item>(Vec3(6000, 600, 2000), true));
	poworStone.push_back(std::make_shared<Item>(Vec3(6000, 500, 2200), true));
	poworStone.push_back(std::make_shared<Item>(Vec3(6000, 100, 1400), true));
	poworStone.push_back(std::make_shared<Item>(Vec3(6000, -500, 2000), true));
	poworStone.push_back(std::make_shared<Item>(Vec3(-3300, 1000, -3300), true));
	m_skyDomeH = ModelManager::GetInstance().GetModelData("Skybox.mv1");
	fontHandle = FontManager::GetInstance().GetFontData("disital.TTF", "Pocket Calculator", 60, 7, DX_FONTTYPE_NORMAL);

	MV1SetScale(m_skyDomeH, VGet(1.3f, 1.3f, 1.3f));

	m_managerUpdate = &Galaxy::IntroUpdate;
	m_managerDraw = &Galaxy::IntroDraw;

	m_miniMapScreenHandle = ScreenManager::GetInstance().GetScreenData(kMiniMapScreenName, Game::kScreenWidth, Game::kScreenHeight);
}

Galaxy::~Galaxy()
{
	planet.clear();
	takobo.clear();
	gorori.clear();
	poworStone.clear();
	warpGate.clear();
}

void Galaxy::Init()
{
	SetGlobalAmbientLight(GetColorF(0.0f, 0.0f, 1.0f, 1.0f));

	player->SetMatrix();//モデルに行列を反映

	// メッシュの数を取ってくる
	auto meshNum = MV1GetMeshNum(modelH);

	VECTOR maxPos = { 0, 0, 0 };
	VECTOR minPos = { 1000, 1000, 1000 };
	bool hasNormalMap = false;
	for (int i = 0; i < meshNum; ++i)
	{
		// 輪切りの時は裏側も描画しないと変になる
		MV1SetMeshBackCulling(modelH, i, DX_CULLING_NONE);

		// モデルの大きさを取得
		auto modelMaxPos = MV1GetMeshMaxPosition(modelH, i);
		maxPos.x = max(maxPos.x, modelMaxPos.x);
		maxPos.y = max(maxPos.y, modelMaxPos.y);
		maxPos.z = max(maxPos.z, modelMaxPos.z);

		auto modelMinPos = MV1GetMeshMinPosition(modelH, i);
		minPos.x = min(minPos.x, modelMinPos.x);
		minPos.y = min(minPos.y, modelMinPos.y);
		minPos.z = min(minPos.z, modelMinPos.z);

		auto vtype = MV1GetTriangleListVertexType(modelH, i);
		if (vtype == DX_MV1_VERTEX_TYPE_NMAP_1FRAME)
		{
			hasNormalMap = true;
		}
	}
	userData->minY = minPos.y;
	userData->maxY = maxPos.y;
	userData->clickedU = 0.0f;
	userData->clickedV = 0.0f;

	// 深度値記録バッファ用RT
	DxLib::SetCreateGraphChannelBitDepth(32);
	DxLib::SetCreateDrawValidGraphChannelNum(1);

	MyEngine::Physics::GetInstance().Entry(player);//物理演算クラスに登録
	MyEngine::Physics::GetInstance().Entry(bossPlanet);//物理演算クラスに登録
	for (auto& item : planet)
	{
		item->Init();
		MyEngine::Physics::GetInstance().Entry(item);//物理演算クラスに登録
	}
	for (auto& item : clearObject)
	{
		item->Init();
		MyEngine::Physics::GetInstance().Entry(item);//物理演算クラスに登録
	}
	for (auto& item : poworStone)
	{
		item->Init();
		MyEngine::Physics::GetInstance().Entry(item);//物理演算クラスに登録
	}
	for (auto& item : takobo)
	{
		item->SetTarget(player);
		MyEngine::Physics::GetInstance().Entry(item);//物理演算クラスに登録
	}

	for (auto& item : gorori)
	{
		MyEngine::Physics::GetInstance().Entry(item);//物理演算クラスに登録
		item->SetTarget(player);
	}


	warpGate.push_back(std::make_shared<WarpGate>(Vec3(800, 0, 300), m_warpEffectHandle));
	warpGate.back()->SetWarpPos(Vec3(6000, 0, 2000));
	MyEngine::Physics::GetInstance().Entry(warpGate.back());

}

void Galaxy::Update()
{
	(this->*m_managerUpdate)();
}

void Galaxy::Draw()
{
	(this->*m_managerDraw)();
}

void Galaxy::IntroUpdate()
{
	MyEngine::Physics::GetInstance().Update();//当たり判定を更新
	Vec3 planetToPlayer = player->GetPos() - player->GetNowPlanetPos();
	Vec3 sideVec = GetCameraRightVector();
	Vec3 front = Cross(planetToPlayer, sideVec).GetNormalized() * -1;
	player->SetSideVec(sideVec);
	player->SetFrontVec(front);
	player->SetUpVec(planetToPlayer);

	//本当はカメラとプレイヤーの角度が90度以内になったときプレイヤーの頭上を見たりできるようにしたい。
	//今回はプレイヤーの相対位置からカメラ位置を指定
	camera->SetUpVec(player->GetNormVec());
	camera->SetCameraPoint(player->GetPos() + (Vec3(GetCameraUpVector()).GetNormalized() * kCameraDistanceUp - front * (kCameraDistanceFront + kCameraDistanceAddFrontInJump * player->GetJumpFlag())));

	camera->Update(player->GetPos());//カメラ視点の更新

	// カリング方向の反転
	for (int i = 0; i < MV1GetMeshNum(modelH); ++i)
	{
		MV1SetMeshBackCulling(modelH, i, DX_CULLING_RIGHT);
	}
	//今回はシェーダーも使用しない


	// カリング方向を元に戻す
	for (int i = 0; i < MV1GetMeshNum(modelH); ++i)
	{
		MV1SetMeshBackCulling(modelH, i, DX_CULLING_LEFT);
	}
}

void Galaxy::IntroDraw()
{
	MV1SetPosition(m_skyDomeH, VGet(0, 0, 0));

	MV1DrawModel(m_skyDomeH);
	bossPlanet->Draw();
	for (auto& item : planet)item->Draw();
	player->Draw();
	for (auto& item : warpGate)
	{
		item->Draw();
	}
	for (auto& item : poworStone)
	{
		item->Draw();
	}
	for (auto& item : takobo)item->Draw();
	//for (auto& item : killerTheSeeker)item->Draw();
	for (auto& item : gorori)item->Draw();
	camera->DebagDraw();
	DxLib::SetRenderTargetToShader(1, -1);		// RTの解除
	DxLib::SetRenderTargetToShader(2, -1);		// RTの解除

}

void Galaxy::GamePlayingUpdate()
{

	camera->Update(player->GetPos());
	Vec3 planetToPlayer = player->GetPos() - player->GetNowPlanetPos();
	Vec3 sideVec = GetCameraRightVector();
	Vec3 front = Cross(planetToPlayer, sideVec).GetNormalized() * -1;//-1をかけて逆ベクトルにしている

	//相対的な軸ベクトルの設定
	player->SetSideVec(sideVec);
	player->SetFrontVec(front);
	player->SetUpVec(planetToPlayer);

	camera->SetBoost(player->GetBoostFlag());
	//本当はカメラとプレイヤーの角度が90度以内になったときプレイヤーの頭上を見たりできるようにしたい。
	camera->SetUpVec(player->GetNormVec());

	if (player->GetBoostFlag())
	{
		camera->SetCameraPoint(player->GetPos() + (Vec3(GetCameraUpVector()).GetNormalized() * (kCameraDistanceUp - 400) - front * ((kCameraDistanceFront - 700) + kCameraDistanceAddFrontInJump * player->GetJumpFlag())));
	}
	else
	{
		camera->SetCameraPoint(player->GetPos() + (Vec3(GetCameraUpVector()).GetNormalized() * kCameraDistanceUp - front * (kCameraDistanceFront + kCameraDistanceAddFrontInJump * player->GetJumpFlag())));

	}

	bossPlanet->Update();//ボスステージの更新
	for (auto& item : planet)item->Update();//ステージの更新

	for (auto& item : poworStone)
	{
		item->Update();//物体Xの更新
	}
	for (auto& item : clearObject)
	{
		item->Update();//グランド物体Xの更新(呼ばれるのはボスを倒した後)
	}
	player->Update();

	for (auto& item : takobo)
	{
		item->Update();//遠距離攻撃する敵の更新
	}
	for (auto& item : killerTheSeeker)
	{
		item->Update();//ボスの更新
	}
	for (auto& item : gorori)item->Update();//惑星を走り回る敵の更新

	userData->dissolveY = player->GetRegenerationRange();//シェーダー用プロパティ

	MyEngine::Physics::GetInstance().Update();//当たり判定の更新

	for (auto& item : warpGate)item->SetEffectPos();//エフェクトの更新
	if (player->GetBoostFlag())
	{
		camera->m_cameraUpdate = &Camera::NeutralUpdate;
	}
	for (int i = 0; i < takobo.size(); i++)
	{
		takobo[i]->DeleteManage();//削除管理
		if (takobo[i]->WatchHp() < 0)
		{
			MyEngine::Physics::GetInstance().Exit(takobo[i]);//物理演算クラスから登録解除

			takobo.erase(takobo.begin() + i);//さっきの例をそのまま使うと(1,2,5,3,4)でitには5まで入ってるので取り除きたい3,4はitからend()までで指定できる
			i--;
		}
	}
	for (auto& item : warpGate)
	{
		item->Update();//加速装置の更新
	}

	for (int i = 0; i < killerTheSeeker.size(); i++)
	{
		killerTheSeeker[i]->DeleteManage();//削除管理
		if (killerTheSeeker[i]->WatchHp() < 300)
		{
			bossPlanet->SetColor(0xff44ff);//ボスステージの色変更
			bossPlanet->SetGravityPower(20.f);//重力変更

		}
		if (killerTheSeeker[i]->WatchHp() < 100)
		{
			bossPlanet->SetColor(0xffaa00);//色変更
			bossPlanet->SetGravityPower(1.f);//重力変更
		}
		if (killerTheSeeker[i]->WatchHp() < 0)
		{
			clearObject.push_back(std::make_shared<ClearObject>(killerTheSeeker[i]->GetRigidbody()->GetPos()));//クリアオブジェクトの生成
			clearObject.back()->Init();
			MyEngine::Physics::GetInstance().Entry(clearObject.back());//物理演算クラスに登録

			camera->WatchThis(clearObject.back()->GetRigidbody()->GetPos(), Vec3(clearObject.back()->GetRigidbody()->GetPos() + (bossPlanet->GetNormVec(clearObject.back()->GetRigidbody()->GetPos()) * 300)), bossPlanet->GetNormVec(clearObject.back()->GetRigidbody()->GetPos()));//カメラの視点管理
			MyEngine::Physics::GetInstance().Exit(killerTheSeeker[i]);//登録解除

			killerTheSeeker.erase(killerTheSeeker.begin() + i);//さっきの例をそのまま使うと(1,2,5,3,4)でitには5まで入ってるので取り除きたい3,4はitからend()までで指定できる
			i--;
		}
	}

	for (int i = 0; i < gorori.size(); i++)
	{
		if (gorori[i]->WatchHp() < 0)//死んでいるか
		{
			MyEngine::Physics::GetInstance().Exit(gorori[i]);//登録解除

			gorori.erase(gorori.begin() + i);//さっきの例をそのまま使うと(1,2,5,3,4)でitには5まで入ってるので取り除きたい3,4はitからend()までで指定できる
			i--;
		}
	}
	for (int i = 0; i < poworStone.size(); i++)
	{
		if (poworStone[i]->GetDeleteFlag())
		{
			MyEngine::Physics::GetInstance().Exit(poworStone[i]);//登録解除

			poworStone.erase(poworStone.begin() + i);//さっきの例をそのまま使うと(1,2,5,3,4)でitには5まで入ってるので取り除きたい3,4はitからend()までで指定できる
			i--;
		}
	}


	player->SetMatrix();//行列を反映
	for (auto& item : takobo)item->SetMatrix();//行列を反映
	// カリング方向の反転
	for (int i = 0; i < MV1GetMeshNum(modelH); ++i)
	{
		MV1SetMeshBackCulling(modelH, i, DX_CULLING_RIGHT);
	}
	// カリング方向を元に戻す
	for (int i = 0; i < MV1GetMeshNum(modelH); ++i)
	{
		MV1SetMeshBackCulling(modelH, i, DX_CULLING_LEFT);
	}

	if (player->GetHp() <= 0)
	{
		m_isGameOverFlag = true;
	}
	if (player->IsClear())
	{
		m_isClearFlag = true;
	}
	if (poworStone.size() <= 5 && warpGate.size() == 0)
	{
		warpGate.push_back(std::make_shared<WarpGate>(Vec3(800, 0, 300), m_warpEffectHandle));
		warpGate.back()->SetWarpPos(Vec3(6000, 0, 2000));
		MyEngine::Physics::GetInstance().Entry(warpGate.back());
		camera->WatchThis(warpGate.back()->GetRigidbody()->GetPos(), Vec3(1600, 0, 600), planet[0]->GetNormVec(warpGate.back()->GetRigidbody()->GetPos()));
	}
	if (poworStone.size() <= 1 && warpGate.size() == 1)
	{
		warpGate.push_back(std::make_shared<WarpGate>(Vec3(5500, 500, 1700), m_warpEffectHandle));
		warpGate.back()->SetWarpPos(Vec3(-3000, 1000, -3000));
		MyEngine::Physics::GetInstance().Entry(warpGate.back());
		camera->WatchThis(warpGate.back()->GetRigidbody()->GetPos(), Vec3(4000, 700, 1000), planet[0]->GetNormVec(warpGate.back()->GetRigidbody()->GetPos()));
	}
	if (poworStone.size() == 0 && warpGate.size() == 2)
	{
		warpGate.push_back(std::make_shared<WarpGate>(Vec3(-2500, 500, -2800), m_warpEffectHandle));
		warpGate.back()->SetWarpPos(Vec3(0, -6000, 0));
		MyEngine::Physics::GetInstance().Entry(warpGate.back());
		camera->WatchThis(warpGate.back()->GetRigidbody()->GetPos(), Vec3(-2000, 0, -2000), planet[0]->GetNormVec(warpGate.back()->GetRigidbody()->GetPos()));
		killerTheSeeker.push_back(std::make_shared<KillerTheSeeker>(Vec3(0, -7000, 0)));//ボス出現
		for (auto& item : killerTheSeeker)
		{
			item->Init();//初期化処理
			MyEngine::Physics::GetInstance().Entry(item);//物理演算クラスに登録
			item->SetTarget(player);//ターゲットを指定
		}
	}
	if (player->GetNowPlanetPos() == Vec3(0, -6000, 0) && !m_isBossWatch)
	{
		m_isBossWatch = true;
		camera->WatchThis(killerTheSeeker.back()->GetMyPos(), killerTheSeeker.back()->GetMyPos() + Vec3(0, 0, -1200), bossPlanet->GetNormVec(killerTheSeeker.back()->GetMyPos()));
	}
	
}

void Galaxy::GamePlayingDraw()
{
	DxLib::SetDrawScreen(m_miniMapScreenHandle);

	ClearDrawScreen();

	SetCameraPositionAndTargetAndUpVec((player->GetPos() + player->GetNormVec() * 300).VGet(), player->GetPos().VGet(), m_cameraUpVec.VGet());
	m_cameraUpVec = GetCameraUpVector();

	Vec3 pos = MV1GetPosition(m_skyDomeH);
	DxLib::MV1DrawModel(m_skyDomeH);

	bossPlanet->SetIsSearch(player->IsSearch());
	bossPlanet->Draw();
	for (auto& item : planet)
	{
		item->SetIsSearch(player->IsSearch());
		item->Draw();
	}

	player->Draw();
	for (auto& item : warpGate)
	{
		item->Draw();
	}
	if (player->IsSearch())
	{
		DxLib::SetDrawBlendMode(DX_BLENDMODE_MUL, 255);
		// ちょっと暗い矩形を描画
		DxLib::DrawBox(0, 0, 1600, 900,
			0x444488, true);
		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	}
	for (auto& item : poworStone)
	{
		item->Draw();
	}
	for (auto& item : clearObject)
	{
		item->Draw();
	}

	for (auto& item : takobo)item->Draw();
	for (auto& item : killerTheSeeker)item->Draw();
	for (auto& item : gorori)item->Draw();

	camera->DebagDraw();
	Effekseer_Sync3DSetting();
	DrawEffekseer3D();

	DxLib::SetRenderTargetToShader(1, -1);		// RTの解除
	DxLib::SetRenderTargetToShader(2, -1);		// RTの解除

	DxLib::SetDrawScreen(DX_SCREEN_BACK);

	camera->Update(player->GetPos());

	DxLib::MV1DrawModel(m_skyDomeH);


	DxLib::DrawRectRotaGraph(kUiText_SrkX, kUiText_SrkY, kUiText_SrkX, kUiText_SrkY, kUiText_Width, kUiText_Height, kUiText_Exrate, 0, textureUIHandle, true);

	bossPlanet->SetIsSearch(player->IsSearch());
	bossPlanet->Draw();
	for (auto& item : planet)
	{
		item->SetIsSearch(player->IsSearch());
		item->Draw();
	}

	player->Draw();
	for (auto& item : warpGate)
	{
		item->Draw();
	}
	if (player->IsSearch())
	{
		DxLib::SetDrawBlendMode(DX_BLENDMODE_MUL, 255);
		// ちょっと暗い矩形を描画
		DxLib::DrawBox(0, 0, 1600, 900,
			0x444488, true);
		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	}
	for (auto& item : poworStone)
	{
		item->Draw();
	}
	for (auto& item : clearObject)
	{
		item->Draw();
	}

	for (auto& item : takobo)item->Draw();
	for (auto& item : killerTheSeeker)item->Draw();
	for (auto& item : gorori)item->Draw();

	camera->DebagDraw();
	Effekseer_Sync3DSetting();
	DrawEffekseer3D();

	DxLib::SetRenderTargetToShader(1, -1);		// RTの解除
	DxLib::SetRenderTargetToShader(2, -1);		// RTの解除

	int alpha = static_cast<int>(255 * (static_cast<float>(player->GetDamageFrame()) / 60.0f));

	DxLib::SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
	DxLib::DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0xff4444, true);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DxLib::DrawExtendGraph(600, 0, 1000, 250, m_miniMapScreenHandle, false);
	DxLib::DrawBox(600, 0, 1000, 250, 0x00dddd, false);
}
