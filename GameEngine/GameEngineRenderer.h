#pragma once
#include "GameEngineActorSubObject.h"
#include "GameEngineEnum.h"
#include <map>

class GameEngineImage;	// ���ϰ��� �Һи��ϱ� ������ ���� x
class GameEngineFolderImage;
class GameEngineRenderer : public GameEngineActorSubObject
{
	friend GameEngineActor;
	friend GameEngineLevel;

public:
	// constrcuter destructer
	GameEngineRenderer();
	~GameEngineRenderer();

	// delete Function
	GameEngineRenderer(const GameEngineRenderer& _Other) = delete;
	GameEngineRenderer(GameEngineRenderer&& _Other) noexcept = delete;
	GameEngineRenderer& operator=(const GameEngineRenderer& _Other) = delete;
	GameEngineRenderer& operator=(GameEngineRenderer&& _Other) noexcept = delete;

	inline void SetTransColor(unsigned int _Color)
	{
		TransColor_ = _Color;
	}

	// �׷��� ��ġ
	inline void SetPivot(const float4 _Pos)
	{
		RenderPivot_ = _Pos;
	}

	// ���� ������ Ÿ�� (CENTER or BOT))
	inline void SetPivotType(const RenderPivot& _Type)
	{
		PivotType_ = _Type;
	}

	// �׷��� �� ũ�� ���� Ÿ��
	inline void SetScaleMode(const RenderScaleMode& _Mode)
	{
		ScaleMode_ = _Mode;
	}

	// ImageŸ�� �̹��� - ũ�� ����
	void SetImageScale();

	// UserŸ�� �̹��� - ũ�� ����
	inline void SetScale(const float4& _Scale)
	{
		ScaleMode_ = RenderScaleMode::User;
		RenderScale_ = _Scale;
	}

	inline GameEngineImage* GetImage()
	{
		return Image_;
	}

	void SetImage(const std::string& _Name);

	void SetIndex(size_t _Index, float4 _Scale = { -1.0f, -1.0f });

	void CameraEffectOff()
	{
		IsCameraEffect_ = false;
	}

	void CameraEffectOn()
	{
		IsCameraEffect_ = true;
	}

	void SetOrder(int _Order) override;

protected:
	void Render();

private:
	friend class FrameAnimation;

	GameEngineImage* Image_;
	RenderPivot PivotType_;		// �̹����� �׷��� ������
	RenderScaleMode ScaleMode_;	// ennum�ɼ� ����ִ� �뵵
	float4 RenderPivot_;		// ����ڰ� ������ ��ġ

	float4 RenderScale_;		// (����ڰ� ������) ȭ�鿡 �׷��� ũ�� -> Ȯ��, ��� ����

	float4 RenderImageScale_;	// �̹������� �߶� ũ��
	float4 RenderImagePivot_;	// �̹����� '������' �߶� �׸��� 

	unsigned int TransColor_;

	bool IsCameraEffect_;

///////////////////////////////////////////////////////////////// �ִϸ��̼�

private:
	class FrameAnimation : public GameEngineNameObject
	{
	public:
		GameEngineRenderer* Renderer_;
		GameEngineImage* Image_;
		GameEngineFolderImage* FolderImage_;

		int CurrentFrame_;
		int StartFrame_;
		int EndFrame_;
		float CurrentInterTime_;
		float InterTime_;
		bool Loop_;
		bool IsEnd;

	public:
		FrameAnimation()
			: Renderer_(nullptr)
			, Image_(nullptr)
			, CurrentFrame_(-1)
			, StartFrame_(-1)
			, EndFrame_(-1)
			, CurrentInterTime_(0.1f)
			, InterTime_(0.1f)
			, Loop_(true)
			, FolderImage_(nullptr)
			, IsEnd(false)

		{

		}

	public:
		void Update();

		void Reset()
		{
			CurrentFrame_ = StartFrame_;
			CurrentInterTime_ = InterTime_;
		}
	};

public:
	void CreateAnimation(const std::string& _Image, const std::string& _Name, int _StartIndex, int _EndIndex, float _InterTime, bool _Loop = true);

	void CreateFolderAnimation(const std::string& _Image, const std::string& _Name, int _StartIndex, int _EndIndex, float _InterTime, bool _Loop = true);

	// ���� �ִϸ��̼��� ����
	void ChangeAnimation(const std::string& _Name);


	bool IsEndAnimation();

	bool IsAnimationName(const std::string& _Name);


private:
	std::map<std::string, FrameAnimation> Animations_;
	FrameAnimation* CurrentAnimation_;



};