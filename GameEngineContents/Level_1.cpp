#include "Level_1.h"
#include <GameEngineBase/GameEngineInput.h>
#include <GameEngine/GameEngine.h>
#include <GameEngine/GameEngineRenderer.h>
#include <GameEngineBase/GameEngineWindow.h>
#include <GameEngine/GameEngineImageManager.h>
#include <GameEngine/GameEngineCollision.h>
#include "BackGround.h"
#include "Stage.h"
#include "Player.h"
#include "ContentsEnum.h"
#include "PlayUI.h"
#include "Monster.h"
#include "WaddleDee.h"

Level_1::Level_1()
{

}
Level_1::~Level_1()
{
}

void Level_1::Loading()
{
	if (nullptr == Player::MainPlayer)
	{
		Player::MainPlayer = CreateActor<Player>((int)ORDER::PLAYER, "Player");

		// UI 
		PlayUI::MainUI = CreateActor<PlayUI>((int)ORDER::UI, "PlayUI");
	}

	// ��׶��� ����
	{
		BackGround* CurBack = CreateActor<BackGround>((int)ORDER::BACKGROUND);
		CurBack->GetRenderer()->SetImage("Back1.bmp");

		float4 BackActor = {};
		BackActor.x = (CurBack->GetRenderer()->GetImage()->GetScale().Half().x);
		BackActor.y = (CurBack->GetRenderer()->GetImage()->GetScale().Half().y);

		CurBack->GetRenderer()->SetPivot(BackActor);
	}


	// ��������
	{
		Stage* CurStage = CreateActor<Stage>((int)ORDER::STAGE);
		CurStage->GetRenderer()->SetImage("Level1.bmp");

		float4 StageActor = {};
		StageActor.x = (CurStage->GetRenderer()->GetImage()->GetScale().Half().x);
		StageActor.y = (CurStage->GetRenderer()->GetImage()->GetScale().Half().y);

		CurStage->GetRenderer()->SetPivot(StageActor);


		CurStage->CreateCollision("Door", { 100, 100 }, { 500, 200 });

		// ��ֹ� �ݸ���
		//CurStage->CreateCollision("Wall", { 100, 100 }, { 300, 200 });
	}


	// ����
	Monster* Waddle = CreateActor<Monster>((int)ORDER::MONSTER, "WaddleDee");
	Waddle->CreateRenderer();

	/*Waddle = dynamic_cast<WaddleDee*>(Waddle);

	Waddle->SetPosition({ 500,500 });
	Waddle->CreateRenderer();*/

	
	

}

void Level_1::Update()
{
	// ���� ü���� 
	if (true == GameEngineInput::GetInst()->IsDown("TitleLevel"))
	{
		GameEngine::GetInst().ChangeLevel("TitleLevel");
	}

	if (true == GameEngineInput::GetInst()->IsDown("Level_2"))
	{
		GameEngine::GetInst().ChangeLevel("Level_2");
	}

	if (true == GameEngineInput::GetInst()->IsDown("NextLevel"))
	{
		GameEngine::GetInst().ChangeLevel("Level_2");
	}
}

void Level_1::LevelChangeStart(GameEngineLevel* _PrevLevel)
{
	//BgmPlayer = GameEngineSound::SoundPlayControl("Play1.mp3");

	//Player::MainPlayer->SetPosition({ 100.f, 520.f });
	Player::MainPlayer->SetPosition({ 1800.f, 420.f });
	//Player::MainPlayer->SetPosition({ 5500.f, 450.f });
	Player::MainPlayer->MapScale(5753.f, 768.f);
}

void Level_1::LevelChangeEnd(GameEngineLevel* _NextLevel)
{
	//BgmPlayer.Stop();

	if (_NextLevel->GetNameCopy() != "TitleLevel")
	{
		Player::MainPlayer->NextLevelOn();
		PlayUI::MainUI->NextLevelOn();
	}
}
