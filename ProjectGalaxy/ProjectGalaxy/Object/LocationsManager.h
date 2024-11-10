#pragma once
#include<string>
#include<vector>
#include"Vec3.h"

class LocationsManager
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

	static LocationsManager& GetInstance();

	void LoadLocations();
	std::vector<LocationData> m_locationData;
};

