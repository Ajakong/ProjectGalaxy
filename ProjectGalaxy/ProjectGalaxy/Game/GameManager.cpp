#include "GameManager.h"

GameManager::GameManager():
	galaxy(std::make_shared<Galaxy>())
{
}

GameManager::~GameManager()
{
}

void GameManager::Init()
{
	galaxy->Init();
}

void GameManager::Update()
{
	galaxy->Update();
}

void GameManager::Draw()
{
	galaxy->Draw();
}

void GameManager::IntroUpdate()
{
}

void GameManager::IntroDraw()
{
}

void GameManager::GamePlayingUpdate()
{
}

void GameManager::GamePlayingDraw()
{
}
