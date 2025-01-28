#pragma once
class Mission
{
public:
	Mission();
	virtual ~Mission();

	void UpDate();
	void Draw();

	void MoveUpdate();
	void MoveDraw();

	//メンバ関数ポインタ
	using missionState_t = void(Mission::*)();
	missionState_t m_missionUpdate;
	missionState_t m_missionDraw;

};

