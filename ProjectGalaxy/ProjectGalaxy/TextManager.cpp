#include "TextManager.h"
#include"FontManager.h"
#include<tchar.h>

TextManager::TextManager()
	:m_fontHandle(FontManager::GetInstance().GetFontData("SF_font.ttf","廻想体 ネクスト UP B",20)),
	m_drawTextLength(0),
	m_drawTextFrame(0)
{
	
}

TextManager::~TextManager()
{
}

void TextManager::Init()
{
}

void TextManager::Update()
{
}

void TextManager::Draw()
{
	m_drawTextFrame++;
	if (m_drawTextFrame > 0)
	{
		m_drawTextFrame = 0;
		if (m_drawTextLength <= m_texts.front().length())
		{
			
			m_drawTextLength++;
		}

	}
	std::string drawText;

	for (int i = 0; i < m_drawTextLength; i++)
	{
		drawText += m_texts.front()[i];
	}


	DrawFormatStringFToHandle(300, 200, 0xffffff, m_fontHandle,drawText.c_str());
}

void TextManager::InText(const std::string text)
{
	m_texts.push_back(text);
}

void TextManager::DeleteText()
{
	m_drawTextLength = 0;
	m_texts.pop_front();
}



