#include "GameEngine.h"
#include "GameEngineBase/GameEngineWindow.h"
#include "GameEngineLevel.h"
#include "GameEngineImageManager.h"
#include <GameEngineBase/GameEngineInput.h>
#include <GameEngineBase/GameEngineTime.h>
#include <GameEngineBase/GameEngineSound.h>

// static�����ϱ� �ʱ�ȭ �ʿ� 
std::map<std::string, GameEngineLevel*> GameEngine::AllLevel_;
GameEngineLevel* GameEngine::CurrentLevel_ = nullptr;
GameEngineLevel* GameEngine::NextLevel_ = nullptr;
GameEngine* GameEngine::UserContents_ = nullptr;
GameEngineImage* GameEngine::BackBufferImage_ = nullptr;
GameEngineImage* GameEngine::WindowMainImage_ = nullptr;


GameEngine::GameEngine()
{

}
GameEngine::~GameEngine()
{
}

HDC GameEngine::BackBufferDC()
{
    return BackBufferImage_->ImageDC();
}

void GameEngine::GameInit()
{
}

void GameEngine::GameLoop()
{
}

void GameEngine::GameEnd()
{
}

void GameEngine::WindowCreate()
{
    // �����찡 ��������鼭 ������ ũ�⸸�� HBITMAP�� ��������� -> �����쿡�� ���ԵǴ� ��Ʈ��
    GameEngineWindow::GetInst().CreateGameWindow(nullptr, "Kirby"); 
    GameEngineWindow::GetInst().ShowGameWindow();
    GameEngineWindow::GetInst().MessageLoop(EngineInit, EngineLoop);
}

void GameEngine::EngineInit()
{
    // �����찡 ��������� �������� ũ�⸦ ������� ������ ũ���� �� ���۸� ���� �� �ִµ�
    // ���⼭ ������ ũ�Ⱑ �����ȴ�
    UserContents_->GameInit();

    
    WindowMainImage_ = GameEngineImageManager::GetInst()->Create("WindowMain", GameEngineWindow::GetHDC());
  
    // ����� ����
    BackBufferImage_ = GameEngineImageManager::GetInst()->Create("BackBuffer", GameEngineWindow::GetScale());
}

void GameEngine::EngineLoop()
{
    GameEngineTime::GetInst()->Update();

    // 1. �������ؿ��� �� �����Ӹ��� üũ 
    UserContents_->GameLoop();

    // ���� ������ �ִ�
    if (nullptr != NextLevel_)
    {
        // ���� ������ ������ -> ���� ���� End
        if (nullptr != CurrentLevel_)
        {
            CurrentLevel_->ActorLevelChangeEnd();
            CurrentLevel_->LevelChangeEnd();
        }
        
        // ���� ���� = ���� ������ �ȴ�
        CurrentLevel_ = NextLevel_;

        // ���� ����(���� ����)�� ������ -> ���� ���� Start
        if (nullptr != CurrentLevel_)
        {
            CurrentLevel_->LevelChangeStart();
            CurrentLevel_->ActorLevelChangeStart();
        }

        NextLevel_ = nullptr;

        // ���� �ε��� ���� �ĸ� 0���� ���� �� �� ���� �ð� üũ
        GameEngineTime::GetInst()->Reset();

        Rectangle(WindowMainImage_->ImageDC(), 0, 0, WindowMainImage_->GetScale().ix(), WindowMainImage_->GetScale().iy());
        Rectangle(BackBufferImage_->ImageDC(), 0, 0, BackBufferImage_->GetScale().ix(), BackBufferImage_->GetScale().iy());
    }

    if (nullptr == CurrentLevel_)
    {
        MsgBoxAssert("Level is nullptr => GameEngine Loop Error");
    }


    GameEngineSound::Update();
    // ���ֵ� ������Ʈ ���� Key������Ʈ
    GameEngineInput::GetInst()->Update(GameEngineTime::GetInst()->GetDeltaTime());

    // 2. ���� ���ؿ��� üũ 
    CurrentLevel_->Update();
    CurrentLevel_->ActorUpdate();
    CurrentLevel_->ActorRender();

    // �ݸ��� 
    //CurrentLevel_->CollisionDebugRender(); 

    // �� ���� �̹����� ���� ������� ����
    WindowMainImage_->BitCopy(BackBufferImage_);

    // Death�ִ��� üũ�ϰ� ���� 
    CurrentLevel_->ActorRelease();
}

void GameEngine::EngineEnd()
{
    UserContents_->GameEnd();

    std::map<std::string, GameEngineLevel*>::iterator StartIter = AllLevel_.begin();
    std::map<std::string, GameEngineLevel*>::iterator EndIter = AllLevel_.end();

    for (; StartIter != EndIter; ++StartIter)
    {
        if (nullptr == StartIter->second)
        {
            continue;
        }
        
        delete StartIter->second;
    }

    GameEngineSound::AllResourcesDestroy();
    GameEngineImageManager::Destroy();

    GameEngineInput::Destroy();
    GameEngineTime::Destroy();
    GameEngineWindow::Destroy();
}


void GameEngine::ChangeLevel(const std::string& _Name)
{
    // ChangeLevel���� �ش� �����ִ��� ���� ��, ������ NextLevel�� �����ȴ�
    // -> �� �� ���� ���� ���� ���� ������ �����Ǿ�� �ϱ� ����
    std::map<std::string, GameEngineLevel*>::iterator FindIter = AllLevel_.find(_Name);

    if (AllLevel_.end() == FindIter)
    {
        MsgBoxAssert("Level Find Error");
        return;
    }

    NextLevel_ = FindIter->second;
}
