#pragma once
#include<string>
#include<vector>
#include"Vec3.h"
// ゲーム全体で使用する定数を定義する
namespace Game
{
	struct LocationData
	{
		std::string name;
		std::string tag;
		Vec3 pos;
		Vec3 rot;
		Vec3 scale;
	};
	constexpr int kScreenWidth = 1600;	// 画面の横幅
	constexpr int kScreenHeight = 900;	// 画面の縦幅

	constexpr int kColorDepth = 32;		// 16 or 32

	void LoadLocations();
	std::vector<LocationData> m_locationData;

}
