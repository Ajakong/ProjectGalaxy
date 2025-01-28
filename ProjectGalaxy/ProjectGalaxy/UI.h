#pragma once
#include<string>
#include<memory>
#include<list>
class TextManager;
class TalkObject;
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
	
	
	void Draw(float m_hp=0);
	
	/// <summary>
	/// 新しいテキスト群をぶち込む
	/// </summary>
	/// <param name="text"></param>
	void InText(const std::string text);
	
	void InTexts(const std::list<std::string> texts);

	void WannaTalk(std::shared_ptr<TalkObject> obj);
	void TalkExit();
	/// <summary>
	/// 入力を受け付けたら表示しているテキストを削除して次のテキストに移る
	/// </summary>
	void DeleteText();

	int TextRemaining();


	using UIstate_t = void(UI::*)();
	UIstate_t m_uiUpdate;
	UIstate_t m_uiDraw;
	
	UIstate_t GetState() { return m_uiUpdate; }
	void TextBoxUpdate();
	void InputAUpdate();

private:
	
	void NormalUpdate();


	
	void AppaerUpdate();
	void FadeOutUpdate();

	void NormalDraw();

	//Aボタンで会話
	void InputAFadeDraw();
	void InputADraw();

	
	void TextBoxFadeDraw();
	void TextBoxDraw();


	/// <summary>
	/// テキストデータ群に表示したいテキストをぶち込む
	/// </summary>
	/// <param name="text">表示したいテキスト</param>
	float m_playerHp;

	int m_appearFrame;

	int m_fadeSpeed;


	int m_uiAssetHandle;
	int m_uiInputAHandle;
	std::shared_ptr<TextManager> m_textManager;
	std::shared_ptr<TalkObject> m_nowTalkObject;

};

