#include "Level_2.h"
#include <GameEngineBase/GameEngineInput.h>
#include <GameEngine/GameEngine.h>
#include <GameEngine/GameEngineRenderer.h>
#include <GameEngine/GameEngineImageManager.h>
#include "BackGround.h"
#include "Stage.h"
#include "Player.h"
#include "ContentsEnum.h"
#include "PlayUI.h"
#include "Grass_2.h"
#include "WaddleDee.h"
#include "BigWaddleDee.h"
#include "Scarfy.h"

Level_2::Level_2()
{

}
Level_2::~Level_2()
{

}

void Level_2::Loading()
{
	// UI 
	//CreateActor<PlayUI>((int)ORDER::UI, "PlayUI");


	// �÷��̾�
	//{
	//	Player* Kirby = CreateActor<Player>((int)ORDER::PLAYER);
	//	Kirby->SetPosition({ 100.f, 490.f });
	//	Kirby->MapScale(5376.f, 768.f);
	//}

	{
		// ��׶���
		BackGround* CurBack = CreateActor<BackGround>(0);
		CurBack->GetRenderer()->SetImage("Back1.bmp");

		float4 BackActor = {};
		BackActor.x = (CurBack->GetRenderer()->GetImage()->GetScale().Half().x);
		BackActor.y = (CurBack->GetRenderer()->GetImage()->GetScale().Half().y);

		CurBack->GetRenderer()->SetPivot(BackActor);
	}


	{
		Stage* CurStage = CreateActor<Stage>((int)ORDER::STAGE);
		CurStage->GetRenderer()->SetImage("Level2.bmp");


		float4 StageActor = {};
		StageActor.x = (CurStage->GetRenderer()->GetImage()->GetScale().Half().x);
		StageActor.y = (CurStage->GetRenderer()->GetImage()->GetScale().Half().y);

		CurStage->GetRenderer()->SetPivot(StageActor);
	}

	{
		Grass_2* Grass1 = CreateActor<Grass_2>((int)ORDER::STAGEACTOR, "Grass1");
		Grass1->SetPosition({ 355.f, 513.f });

	}
	
	// ����
	{
		Scarfy* scarfy = CreateActor<Scarfy>((int)ORDER::MONSTER, "Scarfy");
		scarfy->SetPosition({ 500.f, 400.f });
	}
}

void Level_2::Update()
{
	if (true == GameEngineInput::GetInst()->IsDown("TitleLevel"))
	{
		GameEngine::GetInst().ChangeLevel("TitleLevel");
	}

	if (true == GameEngineInput::GetInst()->IsDown("Level_3"))
	{
		GameEngine::GetInst().ChangeLevel("Level_3");
	}

	if (true == GameEngineInput::GetInst()->IsDown("NextLevel"))
	{
		GameEngine::GetInst().ChangeLevel("Level_3");
	}
}

void Level_2::LevelChangeStart(GameEngineLevel* _PrevLevel)
{
	Player::MainPlayer->SetPosition({ 100.f, 490.f });
	Player::MainPlayer->MapScale(5376.f, 768.f);
}

void Level_2::LevelChangeEnd(GameEngineLevel* _NextLevel)
{
	if (_NextLevel->GetNameCopy() != "TitleLevel")
	{
		Player::MainPlayer->NextLevelOn();
		PlayUI::MainUI->NextLevelOn();
	}

}
