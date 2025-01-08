#include "GalaxyCreater.h"
#include"Player.h"
#include"SpherePlanet.h"
#include"Physics.h"

GalaxyCreater::GalaxyCreater(std::string galaxyname)
{
	m_galaxyName = galaxyname;
}

GalaxyCreater::~GalaxyCreater()
{
}

void GalaxyCreater::ObjectCreate(std::shared_ptr<Player> player)
{
	std::string fileName = "Object.loc";
	//開くファイルのハンドルを取得
	int handle = FileRead_open(fileName.c_str());

	//読み込むオブジェクト数が何個あるか取得
	int dataCnt = 0;
	FileRead_read(&dataCnt, sizeof(dataCnt), handle);
	//読み込むオブジェクト数分の配列に変更する
	m_objectData.resize(dataCnt);

	//配列の数分回す
	for (auto& loc : m_objectData)
	{
		//名前のバイト数を取得する
		byte nameCnt = 0;
		FileRead_read(&nameCnt, sizeof(nameCnt), handle);
		//名前のサイズを変更する
		loc.name.resize(nameCnt);
		//名前を取得する
		FileRead_read(loc.name.data(), sizeof(char) * static_cast<int>(loc.name.size()), handle);

		//タグのバイト数を取得する
		byte tagCnt = 0;
		FileRead_read(&tagCnt, sizeof(tagCnt), handle);
		//タグのサイズを変更する
		loc.tag.resize(tagCnt);
		//タグを取得する
		FileRead_read(loc.tag.data(), sizeof(char) * static_cast<int>(loc.tag.size()), handle);

		//座標を取得する
		FileRead_read(&loc.pos, sizeof(loc.pos), handle);
		//回転を取得する
		FileRead_read(&loc.rot, sizeof(loc.rot), handle);
		//大きさを取得する
		FileRead_read(&loc.scale, sizeof(loc.scale), handle);


	}
	FileRead_close(handle);

	for (auto& item : m_objectData)
	{
		printf(("Name:" + item.name + " Tag:" + item.tag + "\n").c_str());
		printf("Pos:%f,%f,%f\n", item.pos.x, item.pos.y, item.pos.z);
		printf("Rot:%f,%f,%f\n", item.rot.x, item.rot.y, item.rot.z);
		printf("scale:%f,%f,%f\n", item.scale.x, item.scale.y, item.scale.z);
	}

}

void GalaxyCreater::PlanetCreate()
{
	std::string fileName = "Data/Info/Planet.loc";
	//開くファイルのハンドルを取得
	int handle = FileRead_open(fileName.c_str());

	//読み込むオブジェクト数が何個あるか取得
	int dataCnt = 0;
	FileRead_read(&dataCnt, sizeof(dataCnt), handle);
	//読み込むオブジェクト数分の配列に変更する
	m_planetData.resize(dataCnt);

	//配列の数分回す
	for (auto& loc : m_planetData)
	{
		//名前のバイト数を取得する
		byte nameCnt = 0;
		FileRead_read(&nameCnt, sizeof(nameCnt), handle);
		//名前のサイズを変更する
		loc.name.resize(nameCnt);
		//名前を取得する
		FileRead_read(loc.name.data(), sizeof(char) * static_cast<int>(loc.name.size()), handle);
		//座標を取得する
		FileRead_read(&loc.pos, sizeof(loc.pos), handle);
		//色を取得する
		FileRead_read(&loc.color, sizeof(loc.color), handle);
		//重力を取得する
		FileRead_read(&loc.gravityPower, sizeof(loc.gravityPower), handle);
		//モデルハンドルを取得する
		FileRead_read(&loc.modelHandle, sizeof(loc.modelHandle), handle);
		//摩擦力を取得する
		FileRead_read(&loc.coefficientOfFriction, sizeof(loc.coefficientOfFriction), handle);
		//大きさを取得する
		FileRead_read(&loc.scale, sizeof(loc.scale), handle);

		auto planet = std::make_shared<SpherePlanet>(loc.pos, loc.color, loc.gravityPower, loc.modelHandle, loc.coefficientOfFriction, loc.scale);
		MyEngine::Physics::GetInstance().Entry(planet);
	}
	FileRead_close(handle);

}
