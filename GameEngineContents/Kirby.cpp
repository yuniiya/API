#include "Kirby.h"
#include "TitleLevel.h"
#include "Level_1.h"
#include "Level_2.h"
#include "Level_3.h"
#include "Level_4.h"
#include "BossRoomLevel.h"
#include "BossLevel.h"
#include "EndingLevel.h"

#include <GameEngineBase/GameEngineWindow.h>
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngineBase/GameEngineFile.h>
#include <GameEngine/GameEngineImageManager.h>
#include <GameEngineBase/GameEngineInput.h>
#include <GameEngineBase/GameEngineSound.h>


Kirby::Kirby()
{

}
Kirby::~Kirby()
{

}

void Kirby::GameInit()
{
	GameEngineWindow::GetInst().SetWindowScaleAndPosition({0, 0}, { 1024, 768 });	// 4:3

	// ���ҽ� ��������
	ResourceLoad();

	GameEngineImage* KirbyLeft1 = GameEngineImageManager::GetInst()->Find("Default_Left.bmp");
	KirbyLeft1->CutCount(10, 14);
	GameEngineImage* KirbyRight1 = GameEngineImageManager::GetInst()->Find("Default_Right.bmp");
	KirbyRight1->CutCount(10, 14);
	//GameEngineImage* KirbyLeft2 = GameEngineImageManager::GetInst()->Find("Default_Jump_Left.bmp");
	//KirbyLeft2->CutCount(9, 1);
	//GameEngineImage* KirbyRight2 = GameEngineImageManager::GetInst()->Find("Default_Jump_Right.bmp");
	//KirbyRight2->CutCount(9, 1);

	// Ű ����
	if (false == GameEngineInput::GetInst()->IsKey("TitleLevel"))
	{
		GameEngineInput::GetInst()->CreateKey("TitleLevel", 'P');
		GameEngineInput::GetInst()->CreateKey("Level_1", '1');
		GameEngineInput::GetInst()->CreateKey("Level_2", '2');
		GameEngineInput::GetInst()->CreateKey("Level_3", '3');
		GameEngineInput::GetInst()->CreateKey("Level_4", '4');
		GameEngineInput::GetInst()->CreateKey("BossRoomLevel", '5');
		GameEngineInput::GetInst()->CreateKey("BossLevel", '6');
		GameEngineInput::GetInst()->CreateKey("EndingLevel", '7');
	}


	// ���� ���� �� ���� ���� ����
	CreateLevel<TitleLevel>("TitleLevel");
	CreateLevel<Level_1>("Level_1");
	CreateLevel<Level_2>("Level_2");
	CreateLevel<Level_3>("Level_3");
	CreateLevel<Level_4>("Level_4");
	CreateLevel<BossRoomLevel>("BossRoomLevel");
	CreateLevel<BossLevel>("BossLevel");
	CreateLevel<EndingLevel>("EndingLevel");

	ChangeLevel("TitleLevel");
}

void Kirby::GameLoop()
{
}

void Kirby::GameEnd()
{
}

void Kirby::ResourceLoad()
{
	// �� �׶���
	{
		// ���� ���丮
		GameEngineDirectory ResourcesDir;
		ResourcesDir.MoveParent("API");
		ResourcesDir.Move("Resources");
		ResourcesDir.Move("Level");

		std::vector<GameEngineFile> AllImageFileList = ResourcesDir.GetAllFile("bmp");

		for (size_t i = 0; i < AllImageFileList.size(); i++)
		{
			GameEngineImageManager::GetInst()->Load(AllImageFileList[i].GetFullPath());
		}
	}

	// Ŀ�� (Test��)
	//{
	//	GameEngineDirectory ResourcesDir;
	//	ResourcesDir.MoveParent("API");
	//	ResourcesDir.Move("Resources");
	//	ResourcesDir.Move("Player");

	//	std::vector<GameEngineFile> AllImageFileList = ResourcesDir.GetAllFile("bmp");
	//	AllImageFileList = ResourcesDir.GetAllFile("bmp");

	//	for (size_t i = 0; i < AllImageFileList.size(); i++)
	//	{
	//		GameEngineImageManager::GetInst()->Load(AllImageFileList[i].GetFullPath());
	//	}
	//}

	{
		GameEngineDirectory ResourcesDir;
		ResourcesDir.MoveParent("API");
		ResourcesDir.Move("Resources");
		ResourcesDir.Move("Actor");
		ResourcesDir.Move("Kirby");

		std::vector<GameEngineFile> AllImageFileList = ResourcesDir.GetAllFile("bmp");
		AllImageFileList = ResourcesDir.GetAllFile("bmp");

		for (size_t i = 0; i < AllImageFileList.size(); i++)
		{
			GameEngineImageManager::GetInst()->Load(AllImageFileList[i].GetFullPath());
		}
	}

	// ��������
	{
		GameEngineDirectory ResourcesDir;
		ResourcesDir.MoveParent("API");
		ResourcesDir.Move("Resources");
		ResourcesDir.Move("Actor");
		ResourcesDir.Move("Stage");

		std::vector<GameEngineFile> AllImageFileList = ResourcesDir.GetAllFile("bmp");
		AllImageFileList = ResourcesDir.GetAllFile("bmp");

		for (size_t i = 0; i < AllImageFileList.size(); i++)
		{
			GameEngineImageManager::GetInst()->Load(AllImageFileList[i].GetFullPath());
		}
	}

	{
		GameEngineDirectory ResourcesDir;
		ResourcesDir.MoveParent("API");
		ResourcesDir.Move("Resources");
		ResourcesDir.Move("UI");

		std::vector<GameEngineFile> AllImageFileList = ResourcesDir.GetAllFile("bmp");
		AllImageFileList = ResourcesDir.GetAllFile("bmp");

		for (size_t i = 0; i < AllImageFileList.size(); i++)
		{
			GameEngineImageManager::GetInst()->Load(AllImageFileList[i].GetFullPath());
		}
	}

	// BossLevel ���� �̹��� �ε�
	{
		GameEngineDirectory ResourcesDir;
		ResourcesDir.MoveParent("API");
		ResourcesDir.Move("Resources");
		ResourcesDir.Move("Level");

		std::vector<GameEngineFile> PlayerImageFolderList = ResourcesDir.GetAllFolder();
		for (size_t i = 0; i < PlayerImageFolderList.size(); ++i)
		{
			GameEngineImageManager::GetInst()->FolderImageLoad(PlayerImageFolderList[i].GetFullPath());
		}
	}


	// ����
	{
		GameEngineDirectory ResourcesDir;
		ResourcesDir.MoveParent("API");
		ResourcesDir.Move("Resources");
		ResourcesDir.Move("Sound");
		//ResourcesDir.Move("Effect");

		std::vector<GameEngineFile> AllImageFileList = ResourcesDir.GetAllFile();

		for (size_t i = 0; i < AllImageFileList.size(); i++)
		{
			GameEngineSound::LoadRes(AllImageFileList[i].GetFullPath());
		}
	}



	//GameEngineImage* WalkImage = GameEngineImageManager::GetInst()->Find("Test.bmp");
	//WalkImage->CutCount(2, 1);
	//GameEngineImage* Right_1 = GameEngineImageManager::GetInst()->Find("1_Right.bmp");
	//Right_1->Cut({ 128,128 });

	//GameEngineImage* Image = GameEngineImageManager::GetInst()->Find("Test.bmp");
	//Image->Cut({80, 80});


}
