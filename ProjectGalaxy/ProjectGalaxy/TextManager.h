#pragma once
#include<string>
#include<list>
#include <tchar.h>

class TextManager
{
	
public:
	TextManager();
	~TextManager();

	void Init();
	void Update();
	void Draw();

	/// <summary>
	/// テキストデータ群に表示したいテキストをぶち込む(全角でぶち込んでください)
	/// </summary>
	/// <param name="text">表示したいテキスト</param>
	void InText(const std::string text);
	/// <summary>
	/// 入力を受け付けたら表示しているテキストを削除して次のテキストに移る
	/// </summary>
	void DeleteText();

	/// <summary>
	/// 入ってるテキストデータの残量
	/// </summary>
	/// <returns></returns>
	int GetTextDataSize() { return m_texts.size(); }


private:
	int m_fontHandle;
	int m_drawTextLength;
	int m_drawTextFrame;
	std::list<std::string> m_texts;

};

