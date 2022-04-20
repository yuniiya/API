#include "PlayUI.h"
#include <GameEngineBase/GameEngineWindow.h>
#include <GameEngine/GameEngineRenderer.h>
#include <GameEngine/GameEngineImageManager.h>

PlayUI* PlayUI::MainUI = nullptr;

PlayUI::PlayUI()
{

}
PlayUI::~PlayUI()
{

}

void PlayUI::Start()
{
	//// ���͸� ȭ�� �߰��� ���´�.
	//{
	//	float4 Pos = GameEngineWindow::GetScale().Half();
	//	SetPosition(Pos);
	//}


	GameEngineRenderer* PlayerHP = CreateRenderer("Player_HP.bmp");
	PlayerHP->SetPivot({ 305.f,740.f });
	PlayerHP->CameraEffectOff();

	GameEngineRenderer* LifeUI = CreateRenderer("LifeUI.bmp");
	LifeUI->SetPivot({ 257.f,686.f });
	LifeUI->CameraEffectOff();

	GameEngineRenderer* SkillUI = CreateRenderer("Skill_Normal.bmp");
	SkillUI->SetPivot({ 70.f,740.f });
	SkillUI->CameraEffectOff();

	//GameEngineRenderer* MonsterHP = CreateRenderer(".bmp");

}

void PlayUI::Update()
{
}



