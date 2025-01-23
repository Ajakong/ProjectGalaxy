#pragma once
#include<string>
#include<memory>
class TextManager;

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
	static UI& GetInstance();
	void Init();
	void Update();
	
	
	void Draw(float m_hp);
	
	void InText(const std::string text);
	/// <summary>
	/// 入力を受け付けたら表示しているテキストを削除して次のテキストに移る
	/// </summary>
	void DeleteText();

	using UIstate_t = void(UI::*)();
	UIstate_t m_uiUpdate;
	UIstate_t m_uiDraw;

private:
	void NormalUpdate();

	void AppaerUpdate();
	void TextBoxUpdate();
	void FadeOutUpdate();

	void NormalDraw();

	void FadeDraw();
	void TextBoxDraw();
	/// <summary>
	/// テキストデータ群に表示したいテキストをぶち込む
	/// </summary>
	/// <param name="text">表示したいテキスト</param>
	float m_playerHp;

	int m_textBoxFrame;



	int m_uiAssetHandle;
	std::shared_ptr<TextManager> m_textManager;

};

