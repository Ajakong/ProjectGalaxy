#pragma once
#include"MyLib/Vec3.h"
#include<memory>
#include<vector>

class Player;

class Galaxy//GameManagerが管理,惑星やオブジェクトの情報を持っている
{
public:
	Galaxy(std::shared_ptr<Player> player);
	virtual ~Galaxy();

	virtual void Init()=0;
	virtual void Update()=0;
	virtual void Draw()=0;
	
	bool GetGameOver() const { return m_isGameOverFlag; }
	bool GetClear() const { return m_isClearFlag; }

	using managerState_t = void(Galaxy::*)();
	managerState_t m_managerUpdate;

	using managerState_t = void(Galaxy::*)();
	managerState_t m_managerDraw;

protected:
	std::shared_ptr<Player> player;

	//このフラグを見てシーンを移行する
	bool m_isGameOverFlag = false;
	bool m_isClearFlag = false;	
};

