#pragma once
#include<string>
#include<vector>
#include"Vec3.h"
// ゲーム全体で使用する定数を定義する
static class Game
{
public:
	struct LocationData
	{
		std::string name;
		std::string tag;
		Vec3 pos;
		Vec3 rot;
		Vec3 scale;
	};
	static const int kScreenWidth = 1600;	// 画面の横幅
	static const int kScreenHeight = 900;	// 画面の縦幅

	static const int kColorDepth = 32;		// 16 or 32

	void LoadLocations();
	std::vector<LocationData> m_locationData;

};
