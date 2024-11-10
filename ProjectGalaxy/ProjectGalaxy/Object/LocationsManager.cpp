#include "LocationsManager.h"
#include"DxLib.h"
LocationsManager& LocationsManager::GetInstance()
{
	static LocationsManager info;
	return info;
}
void LocationsManager::LoadLocations()
{
	int handle = FileRead_open("Data/Info/data.loc");
	int dataCnt = 0;
	FileRead_read(&dataCnt, sizeof(dataCnt), handle);
	m_locationData.resize(dataCnt);
	for (auto& loc : m_locationData)
	{
		//オブジェクト名ロード
		byte nameCnt = 0;
		FileRead_read(&nameCnt, sizeof(nameCnt), handle);
		loc.name.resize(nameCnt);
		FileRead_read(loc.name.data(), sizeof(char) * loc.name.size(), handle);
		//タグ
		FileRead_read(loc.tag.data(), sizeof(char) * loc.tag.size(), handle);
		//座標情報
		FileRead_read(&loc.pos, sizeof(loc.pos), handle);

		//回転情報
		FileRead_read(&loc.rot, sizeof(loc.rot), handle);

		//スケール情報
		FileRead_read(&loc.pos, sizeof(loc.scale), handle);

	}
	FileRead_close(handle);
}
