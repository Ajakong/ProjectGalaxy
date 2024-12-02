#pragma once

class UI
{
	
public:
	struct UIinfo
	{
		float x;
		float y;
		float width;
		float height;
	};
	UI();
	virtual ~UI();

	void Init();
	void Update();
	void Draw();

private:

	int m_uiAsset3Handle;

};

