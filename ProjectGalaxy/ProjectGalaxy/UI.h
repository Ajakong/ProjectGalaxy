﻿#pragma once
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
		int x;
		int y;
		int width;
		int height;
	};

	enum class TalkGraphKind
	{
		TakasakiTaisa,
		Dekahead_Red,
		Dekahead_Green,
		Dekahead_Yellow,
		Dekehead_Blue,
		Dekahead_White,
		Boss
	};
	UI();
	virtual ~UI();
	static UI& GetInstance();
	void Init();
	void Update();
	
	
	void Draw(float hp=0.f,bool aimFlag=false);
	
	/// <summary>
	/// 話すオブジェクトのハンドルを設定
	/// </summary>
	/// <param name="handle"></param>
	void SetTalkObjectHandle(TalkGraphKind obj);
	/// <summary>
	/// 新しいテキストをぶち込む
	/// </summary>
	/// <param name="text">1行で表示したいテキスト</param>
	void InText(const std::string text);
	/// <summary>
	/// 新しいテキスト群をぶち込む
	/// </summary>
	/// <param name="texts">同じテキストボックス内で表示したいテキスト</param>
	void InTexts(const std::list<std::string> texts);
	/// <summary>
	/// TalkObjectが話す
	/// </summary>
	/// <param name="obj">話させたいオブジェクト</param>
	void WannaTalk(std::shared_ptr<TalkObject> obj);
	/// <summary>
	/// 会話にならない
	/// </summary>
	void TalkExit();
	/// <summary>
	/// 入力を受け付けたら表示しているテキストを削除して次のテキストに移る
	/// </summary>
	void DeleteText();
	/// <summary>
	/// 残りテキスト量を取得
	/// </summary>
	/// <returns>残りテキスト量</returns>
	int TextRemaining();


	using UIstate_t = void(UI::*)();
	UIstate_t m_uiUpdate;
	UIstate_t m_uiDraw;
	
	/// <summary>
	/// 現在のモード
	/// </summary>
	/// <returns>モード</returns>
	UIstate_t GetState() { return m_uiUpdate; }

	void TextBoxUpdate();
	void InputAUpdate();

private:
	/// <summary>
	/// テキストモードに移行
	/// </summary>
	void TextMode();
	/// <summary>
	/// Aボタン表示モードに移行
	/// </summary>
	void InputAMode();
	/// <summary>
	/// 通常UIモードに移行
	/// </summary>
	void NormalMode();

	/// <summary>
	/// 通常モード用
	/// </summary>
	void NormalUpdate();
	void NormalDraw();
	
	/// <summary>
	/// フェード用
	/// </summary>
	void AppaerUpdate();
	void FadeOutUpdate();

	/// <summary>
	/// Aボタン表示用
	/// </summary>
	void InputAFadeDraw();
	void InputADraw();
	
	/// <summary>
	/// テキスト表示用
	/// </summary>
	void TextBoxFadeDraw();
	void TextBoxDraw();
	/// <summary>
	/// ミッション描画用
	/// </summary>
	void MissionUpdate();
	void MissionDraw();

	float m_playerHp;

	int m_appearFrame;

	int m_fadeSpeed;

	int m_HPColor;

	/// <summary>
	/// 今話している人の顔画像ハンドル
	/// </summary>
	int m_uiTalkingCharaHandle;
	/// <summary>
	/// 大佐の顔画像ハンドル
	/// </summary>
	int m_uiTakasakiTaisaHandle;
	
	int m_uiDekahead_RedHandle;
	int m_uiDekahead_GreenHandle;
	int m_uiDekahead_YellowHandle;
	int m_uiDekahead_BlueHandle;
	int m_uiDekahead_WhiteHandle;

	int m_uiBossHandle;

	/// <summary>
	/// 常駐UIアセットハンドル
	/// </summary>
	int m_uiAssetHandle;
	/// <summary>
	/// Aボタン画像ハンドル
	/// </summary>
	int m_uiInputAHandle;
	/// <summary>
	/// 照準画像ハンドル
	/// </summary>
	int m_uiAimGraphHandle;

	/// <summary>
	/// テキストボックス出現音
	/// </summary>
	int m_textBoxSEHandle;

	/// <summary>
	/// 会話UI出現音
	/// </summary>
	int m_chatAppearSEHandle;
	/// <summary>
	/// HPが少ないことを知らせる音
	/// </summary>
	int m_hpLowerSEHandle;


	std::shared_ptr<TextManager> m_textManager;
	std::shared_ptr<TalkObject> m_nowTalkObject;

};

