﻿#pragma once
#include <windows.h>


struct Size
{
	int w;
	int h;
};

/// <summary>
/// アプリケーション全体を管理するクラス
/// シングルトンクラス
/// </summary>
class Application
{
public:
	/// <summary>
	/// Applicationのシングルトンインスタンスを返す
	/// </summary>
	/// <returns>Applicationインスタンスの参照を返す</returns>
	static Application& GetInstance()
	{
		static Application instance;	// 自分自身の静的オブジェクトを作ります。
		return instance;
	}

	bool Init();
	void Run();

	float GetGravity() const;

	int GetScreenHandle();

	const Size& GetWindowSize() const;

private:
	Application();	// シングルトンのためにprivateに
	// コピー＆代入の禁止
	Application(const Application& app) = delete;		// コピーコンストラクタの廃止
	void operator =(const Application& app) = delete;	// 代入コンストラクタの廃止

	/// <summary>
	/// 後処理を行う
	/// </summary>
	void Terminate();
private:
	Size m_windowSize;

	int m_screenHandle = 0;


};

