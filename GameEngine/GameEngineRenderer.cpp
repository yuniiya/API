#include "GameEngineRenderer.h"
#include "GameEngineImageManager.h"
#include "GameEngine.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEngineBase/GameEngineTime.h>
#include <GameEngine/GameEngineLevel.h>

#pragma comment(lib, "msimg32.lib")

GameEngineRenderer::GameEngineRenderer()
	: Image_(nullptr)
	, PivotType_(RenderPivot::CENTER)
	, ScaleMode_(RenderScaleMode::Image)	// �⺻ ũ��� ����
	, TransColor_(RGB(255, 0, 255))			// ����Ÿ
	, RenderImagePivot_({0,0})
	, RenderImageScale_({0,0})
	, CurrentAnimation_(nullptr)
	, IsCameraEffect_(true)			// �⺻�� true�� �س��� UIó�� ī�޶� ���� �� �޴� �ֵ��� ���� ����
{

}
GameEngineRenderer::~GameEngineRenderer()
{

}

void GameEngineRenderer::SetImageScale()
{
	if (nullptr == Image_)
	{
		MsgBoxAssert("�������� �ʴ� �̹����� ũ�⸦ �����Ϸ� �߽��ϴ�");
		return;
	}

	// �̹��� ũ�� �״�� ����
	ScaleMode_ = RenderScaleMode::Image;

	RenderScale_ = Image_->GetScale();
	RenderImageScale_ = Image_->GetScale();
}

void GameEngineRenderer::SetImage(const std::string& _Name)
{
	GameEngineImage* FindImage = GameEngineImageManager::GetInst()->Find(_Name);
	if (nullptr == FindImage)
	{
		MsgBoxAssertString(_Name + "�������� �ʴ� �̹����� �����Ϸ� �߽��ϴ�");
	}

	Image_ = FindImage;
	SetImageScale();
}

// Renderer�� �̹����� ���õǾ��־�� ���� ���� -> ���õ� �̹��� ����
void GameEngineRenderer::Render()
{
	// �ִϸ��̼��� ���Դٸ� �̹����� �ִϸ��̼����� ����
	if (nullptr != CurrentAnimation_)
	{
		CurrentAnimation_->Update();
	}

	if (nullptr == Image_)
	{
		MsgBoxAssert("�������� �̹����� ���õǾ� ���� ������ �������� �ȵ˴ϴ�.");
		return;
	}

	// ������ ��ġ
	float4 RenderPos = GetActor()->GetPosition() + RenderPivot_;

	// ī�޶� ����޴� ���Ͷ�� �̵���ǥ�� �ݴ�� 
	if (true == IsCameraEffect_)
	{
		RenderPos -= GetActor()->GetLevel()->GetCameraPos();
	}

	switch (PivotType_)
	{
	case RenderPivot::CENTER:
		GameEngine::BackBufferImage()->TransCopy(Image_, RenderPos - RenderScale_.Half(), RenderScale_, RenderImagePivot_, RenderImageScale_, TransColor_);
		break;
	case RenderPivot::BOT:
	{
		// ���Ϳ���
		float4 Scale = RenderScale_.Half();
		// ���� �߽����� ������ 
		Scale.y *= 2.0f;

		GameEngine::BackBufferImage()->TransCopy(Image_, RenderPos - Scale, RenderScale_, RenderImagePivot_, RenderImageScale_, TransColor_);
		break;
	}	
	default:
		break;
	}
}

void GameEngineRenderer::SetIndex(size_t _Index, const float4 _Scale)
{
	// �߷��� �ִ� �̹����� ���� �� �ְ�
	if (false == Image_->IsCut())
	{
		MsgBoxAssert("�̹����� �κ������� ����Ҽ� �ְ� �߷������� ���� �̹��� �Դϴ�.");
		return;
	}

	// ũ�⸦ ���� �����ߴٸ�
	if (_Scale.x <= 0 || _Scale.y <= 0)
	{
		// �󸶳� �߶�ͼ� ȭ�鿡 �׸��� 
		RenderScale_ = Image_->GetCutScale(_Index);
	}
	else
	{
		RenderScale_ = _Scale;
	}

	// ������ �󸶳� �ڸ���
	RenderImagePivot_ = Image_->GetCutPivot(_Index);
	RenderImageScale_ = Image_->GetCutScale(_Index);

}

/////////////////////////////////////// �ִϸ��̼�

void GameEngineRenderer::CreateFolderAnimation(const std::string& _Image, const std::string& _Name, int _StartIndex, int _EndIndex, float _InterTime, bool _Loop /*= true*/)
{
	GameEngineFolderImage* FindImage = GameEngineImageManager::GetInst()->FolderImageFind(_Image);
	if (nullptr == FindImage)
	{
		MsgBoxAssertString(_Name + "�������� �ʴ� �̹����� �ִϸ��̼��� ������� �߽��ϴ�.");
		return;
	}

	if (Animations_.end() != Animations_.find(_Name))
	{
		MsgBoxAssert("�̹� �����ϴ� �ִϸ��̼��� �� ������� �߽��ϴ�.");
		return;
	}

	FrameAnimation& NewAnimation = Animations_[_Name];

	NewAnimation.SetName(_Name);
	NewAnimation.Renderer_ = this;
	NewAnimation.FolderImage_ = FindImage;
	NewAnimation.CurrentFrame_ = _StartIndex;
	NewAnimation.StartFrame_ = _StartIndex;
	NewAnimation.EndFrame_ = _EndIndex;
	NewAnimation.CurrentInterTime_ = _InterTime;
	NewAnimation.InterTime_ = _InterTime;
	NewAnimation.Loop_ = _Loop;

}



void GameEngineRenderer::ChangeAnimation(const std::string& _Name)
{
	std::map<std::string, FrameAnimation>::iterator FindIter = Animations_.find(_Name);

	if (Animations_.end() == FindIter)
	{
		MsgBoxAssert("�������� �ʴ� �ִϸ��̼����� ü���� �Ϸ��� �߽��ϴ�.");
		return;
	}

	CurrentAnimation_ = &FindIter->second;
}


void GameEngineRenderer::CreateAnimation(
	const std::string& _Image,
	const std::string& _Name,
	int _StartIndex,
	int _EndIndex,
	float _InterTime,
	bool _Loop /*= true*/)
{
	GameEngineImage* FindImage = GameEngineImageManager::GetInst()->Find(_Image);

	if (nullptr == FindImage)
	{
		MsgBoxAssert("�������� �ʴ� �̹����� �ִϸ��̼��� ������� �߽��ϴ�.");
		return;
	}

	if (Animations_.end() != Animations_.find(_Name))
	{
		MsgBoxAssert("�̹� �����ϴ� �ִϸ��̼��� �� ������� �߽��ϴ�..");
		return;
	}

	//FrameAnimation Animation;
	//Animation.insert(std::make_pair(, FrameAnimation()));
	
	// ������� �Դٸ� �ִϸ��̼��� ������ �ִٴ� ���̱� ������ find�� insert���ÿ� ó���ؼ� �ٷ� �����
	FrameAnimation& NewAnimation = Animations_[_Name];
	NewAnimation.Renderer_ = this;
	NewAnimation.Image_ = FindImage;
	NewAnimation.CurrentFrame_ = _StartIndex;
	NewAnimation.StartFrame_ = _StartIndex;
	NewAnimation.EndFrame_ = _EndIndex;
	NewAnimation.CurrentInterTime_ = _InterTime;
	NewAnimation.InterTime_ = _InterTime;
	NewAnimation.Loop_ = _Loop;


}

void GameEngineRenderer::FrameAnimation::Update()
{
	// End �ʱⰪ
	IsEnd = false;
	CurrentInterTime_ -= GameEngineTime::GetInst()->GetDeltaTime();
	if (0 >= CurrentInterTime_)
	{
		CurrentInterTime_ = InterTime_;
		++CurrentFrame_;

		if (EndFrame_ < CurrentFrame_)
		{
			if (true == Loop_)
			{
				IsEnd = true;
				CurrentFrame_ = StartFrame_;	// Loop�� True��� �̹����� �ݺ���Ų��.
			}
			else
			{
				IsEnd = true;
				CurrentFrame_ = EndFrame_;		// Loop�� false��� �ִϸ��̼� ������ EndFrame���� ������Ų��.
			}
		}
	}

	if (nullptr != Image_)
	{
		Renderer_->Image_ = Image_;		// ���������� �� �ִϸ��̼� ���鶧 �����߶� �̹����� �������ش�.
		Renderer_->SetIndex(CurrentFrame_);	// ���������� �ε����� �������ش�. ��, �ش� �ִϸ��̼� �̹����� ���° ĭ(Index) �������ָ� �������� �˾Ƽ� ����Ѵ�.
	}
	else if (nullptr != FolderImage_)
	{
		Renderer_->Image_ = FolderImage_->GetImage(CurrentFrame_);		// ���������� �� �ִϸ��̼� ���鶧 �����߶� �̹����� �������ش�.
		Renderer_->SetImageScale();	// ���������� �ε����� �������ش�. ��, �ش� �ִϸ��̼� �̹����� ���° ĭ(Index) �������ָ� �������� �˾Ƽ� ����Ѵ�.
	}
}

// �������� �����ϴ� AllRender�� Order�� ����+�߰��ϴ� ��ɱ��� �߰�
void GameEngineRenderer::SetOrder(int _Order)
{
	if (nullptr == GetActor())
	{
		MsgBoxAssert("���Ͱ� ���õ��� �ʽ��ϴ�.");
	}

	if (nullptr == GetActor()->GetLevel())
	{
		MsgBoxAssert("������ ���õ��� �ʾҽ��ϴ�.");
	}

	if (_Order == GetOrder())
	{
		return;
	}

	GetActor()->GetLevel()->ChangeRenderOrder(this, _Order);
}

bool GameEngineRenderer::IsEndAnimation()
{
	return CurrentAnimation_->IsEnd;
}

bool GameEngineRenderer::IsAnimationName(const std::string& _Name)
{
	return CurrentAnimation_->GetNameConstRef() == _Name;
}