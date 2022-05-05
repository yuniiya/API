#include "SparkKirby.h"
#include "Player.h"

#include <GameEngine/GameEngine.h>
#include <GameEngineBase/GameEngineWindow.h>
#include <GameEngine/GameEngineImageManager.h>
#include <GameEngineBase/GameEngineInput.h>
#include <GameEngineBase/GameEngineTime.h>
#include <GameEngine/GameEngineRenderer.h>
#include <GameEngine/GameEngineCollision.h>
#include <GameEngineBase/GameEngineString.h>
#include <GameEngineBase/GameEngineSound.h>
#include <GameEngine/GameEngineLevel.h>

#include "Stage.h"
#include "ContentsEnum.h"
#include "Monster.h"
#include "Scarfy.h"
#include "Effect_Slide.h"
#include "Effect_RunToStop.h"
#include "Effect_Star.h"
#include "Effect_Exhale.h"

SparkKirby* SparkKirby::SparkPlayer = nullptr;

SparkKirby::SparkKirby()
	: CurSkill_(KirbySkill::Spark)
	, Speed_(350.0f)
	, JumpPower_(1000.f)
	, Gravity_(1500.f)
	, StopTime_(1.f)
	, DownTime_(0.5f)
{

}
SparkKirby::~SparkKirby()
{

}

void SparkKirby::MonsterColCheck()
{
	std::vector<GameEngineCollision*> ColList;

	if (true == PlayerCollision->CollisionResult("DefaultMonster", ColList, CollisionType::Rect, CollisionType::Rect))
	{
		SparkPlayer->Off();

		MainPlayer->SetPosition(GetPosition());
		CurSkill_ = KirbySkill::Default;
		MainPlayer->On();
	}
}

void SparkKirby::AttackColCheck()
{
}

void SparkKirby::LevelChangeStart(GameEngineLevel* _PrevLevel)
{
	SparkPlayer = this;
}

void SparkKirby::Start()
{
	PlayerCollision = CreateCollision("PlayerHitBox", { 70, 70 });
	SparkAttackCollision_ = CreateCollision("SparkAttackCol", { 150.f,150.f });
	SparkAttackCollision_->Off();

	// �ִϸ��̼��� �ϳ��� ����� �ִϸ��̼��� ����ȴ�.
	PlayerAnimationRender = CreateRenderer();
	PlayerAnimationRender->SetPivotType(RenderPivot::BOT);
	PlayerAnimationRender->SetPivot({ -4.f, 250.f });

	// Left
	{
		PlayerAnimationRender->CreateAnimation("Spark_Left.bmp", "Idle_Left", 0, 7, 0.3f, true);
		PlayerAnimationRender->CreateAnimation("Spark_Left.bmp", "Down_Left", 8, 15, 0.3f, true);
		PlayerAnimationRender->CreateAnimation("Spark_Left.bmp", "Slide_Left", 17, 19, 0.07f, true);
		PlayerAnimationRender->CreateAnimation("Spark_Left.bmp", "Jump_Left", 20, 29, 0.03f, false);
		PlayerAnimationRender->CreateAnimation("Spark_Left.bmp", "Walk_Left", 31, 50, 0.033f, true);
		PlayerAnimationRender->CreateAnimation("Spark_Left.bmp", "Run_Left", 51, 58, 0.06f, true);
		PlayerAnimationRender->CreateAnimation("Spark_Left.bmp", "RunToStop_Left", 59, 59, 0.06f, true);

		// Float
		PlayerAnimationRender->CreateAnimation("Spark_Left.bmp", "Float_Left", 61, 65, 0.05f, true);
		PlayerAnimationRender->CreateAnimation("Spark_Left.bmp", "Float_Left_Loop", 66, 90, 0.06f, true);

		// Exhale
		PlayerAnimationRender->CreateAnimation("Spark_Left.bmp", "Exhale_Left", 95, 96, 0.05f, true);

		// Fall
		PlayerAnimationRender->CreateAnimation("Spark_Left.bmp", "Fall_Left", 97, 105, 0.1f, false);
		PlayerAnimationRender->CreateAnimation("Spark_Left.bmp", "FallToBounce_Left", 106, 109, 0.02f, false);
		PlayerAnimationRender->CreateAnimation("Spark_Left.bmp", "BounceToIdle_Left", 110, 112, 0.2f, false);

		// Enter
		PlayerAnimationRender->CreateAnimation("Spark_Left.bmp", "Enter_Left", 133, 137, 0.01f, true);

		// Attack
		PlayerAnimationRender->CreateAnimation("Spark_Left.bmp", "AttackStart_Left", 138, 138, 0.05f, false);
		PlayerAnimationRender->CreateAnimation("Spark_Attack.bmp", "Attack_Left", 0, 3, 0.02f, true);
	}


	// Right
	{
		PlayerAnimationRender->CreateAnimation("Spark_Right.bmp", "Idle_Right", 0, 7, 0.3f, true);
		PlayerAnimationRender->CreateAnimation("Spark_Right.bmp", "Down_Right", 8, 15, 0.3f, true);
		PlayerAnimationRender->CreateAnimation("Spark_Right.bmp", "Slide_Right", 17, 19, 0.07f, true);
		PlayerAnimationRender->CreateAnimation("Spark_Right.bmp", "Jump_Right", 20, 29, 0.03f, false);
		PlayerAnimationRender->CreateAnimation("Spark_Right.bmp", "Walk_Right", 31, 50, 0.033f, true);
		PlayerAnimationRender->CreateAnimation("Spark_Right.bmp", "Run_Right", 51, 58, 0.06f, true);
		PlayerAnimationRender->CreateAnimation("Spark_Right.bmp", "RunToStop_Right", 59, 59, 0.09f, true);

		// Float
		PlayerAnimationRender->CreateAnimation("Spark_Right.bmp", "Float_Right", 61, 65, 0.05f, true);
		PlayerAnimationRender->CreateAnimation("Spark_Right.bmp", "Float_Right_Loop", 66, 90, 0.06f, true);

		// Exhale
		PlayerAnimationRender->CreateAnimation("Spark_Right.bmp", "Exhale_Right", 95, 96, 0.05f, true);

		// Fall
		PlayerAnimationRender->CreateAnimation("Spark_Right.bmp", "Fall_Right", 97, 105, 0.1f, false);
		PlayerAnimationRender->CreateAnimation("Spark_Right.bmp", "FallToBounce_Right", 106, 109, 0.02f, false);
		PlayerAnimationRender->CreateAnimation("Spark_Right.bmp", "BounceToIdle_Right", 110, 112, 0.2f, false);

		// Enter
		PlayerAnimationRender->CreateAnimation("Spark_Right.bmp", "Enter_Right", 133, 137, 0.01f, true);

		// Attack
		PlayerAnimationRender->CreateAnimation("Spark_Right.bmp", "AttackStart_Right", 138, 138, 0.05f, false);
		PlayerAnimationRender->CreateAnimation("Spark_Attack.bmp", "Attack_Right", 0, 3, 0.02f, true);
	}

	AnimationName_ = "Idle_";
	PlayerAnimationRender->ChangeAnimation("Idle_Right");


	Off();
	SparkPlayer = this;
}

void SparkKirby::Update()
{
	ColMapUpdate();

	DoorPixelCheck();

	DirAnimationCheck();
	PlayerStateUpdate();
	MonsterColCheck();

	DebugModeSwitch();
	DebugKirbySkillChange();

	// ī�޶� ��ġ ����
	CameraFix();

}

void SparkKirby::ChangeState(PlayerState _State)
{
	if (CurState_ != _State)
	{
		switch (_State)
		{
		case PlayerState::Idle:
			IdleStart();
			break;
		case PlayerState::Walk:
			WalkStart();
			break;
		case PlayerState::Run:
			RunStart();
			break;
		case PlayerState::RunToStop:
			RunToStopStart();
			break;
		case PlayerState::Down:
			DownStart();
			break;
		case PlayerState::Slide:
			SlideStart();
			break;
		case PlayerState::Jump:
			JumpStart();
			break;
		case PlayerState::Float:
			FloatStart();
			break;
		case PlayerState::Fall:
			FallStart();
			break;
		case PlayerState::FallToBounce:
			FallToBounceStart();
			break;
		case PlayerState::BounceToIdle:
			BounceToIdleStart();
			break;
		case PlayerState::Exhale:
			ExhaleStart();
			break;
		case PlayerState::AttackStart:
			AttackStartStart();
			break;
		case PlayerState::Attack:
			AttackStart();
			break;
		case PlayerState::AttackEnd:
			AttackEndStart();
			break;
		}
	}

	CurState_ = _State;
}

void SparkKirby::PlayerStateUpdate()
{
	switch (CurState_)
	{
	case PlayerState::Idle:
		IdleUpdate();
		break;
	case PlayerState::Walk:
		WalkUpdate();
		break;
	case PlayerState::Run:
		RunUpdate();
		break;
	case PlayerState::RunToStop:
		RunToStopUpdate();
		break;
	case PlayerState::Down:
		DownUpdate();
		break;
	case PlayerState::Slide:
		SlideUpdate();
		break;
	case PlayerState::Jump:
		JumpUpdate();
		break;
	case PlayerState::Float:
		FloatUpdate();
		break;
	case PlayerState::Fall:
		FallUpdate();
		break;
	case PlayerState::FallToBounce:
		FallToBounceUpdate();
		break;
	case PlayerState::BounceToIdle:
		BounceToIdleUpdate();
		break;
	case PlayerState::Exhale:
		ExhaleUpdate();
		break;
	case PlayerState::AttackStart:
		AttackStartUpdate();
		break;
	case PlayerState::Attack:
		AttackUpdate();
		break;
	case PlayerState::AttackEnd:
		AttackEndUpdate();
		break;
	}
}

void SparkKirby::DirAnimationCheck()
{
	std::string ChangeName;

	PlayerDir CheckDir_ = CurDir_;
	//ChangeDirText_ = "Right";

	if (true == GameEngineInput::GetInst()->IsPress("MoveRight"))
	{
		CheckDir_ = PlayerDir::Right;
		ChangeDirText_ = "Right";
	}

	if (true == GameEngineInput::GetInst()->IsPress("MoveLeft"))
	{
		CheckDir_ = PlayerDir::Left;
		ChangeDirText_ = "Left";
	}

	if (true == GameEngineInput::GetInst()->IsPress("Down"))
	{
		if (CheckDir_ == PlayerDir::Left)
		{
			ChangeDirText_ = "Left";
		}
		else
		{
			ChangeDirText_ = "Right";
		}
	}

	if (true == GameEngineInput::GetInst()->IsPress("Attack"))
	{
		if (CheckDir_ == PlayerDir::Left)
		{
			ChangeDirText_ = "Left";
		}
		else
		{
			ChangeDirText_ = "Right";
		}
	}

	if (true == GameEngineInput::GetInst()->IsPress("JumpLeft"))
	{
		if (CheckDir_ == PlayerDir::Left)
		{
			ChangeDirText_ = "Left";
		}
		else
		{
			ChangeDirText_ = "Right";
		}
	}

	if (true == GameEngineInput::GetInst()->IsPress("JumpRight"))
	{
		if (CheckDir_ == PlayerDir::Left)
		{
			ChangeDirText_ = "Left";
		}
		else
		{
			ChangeDirText_ = "Right";
		}
	}

	if (CheckDir_ != CurDir_)
	{
		PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
		CurDir_ = CheckDir_;
	}
}


void SparkKirby::IdleUpdate()
{
	RunningTime_ -= GameEngineTime::GetDeltaTime();

	if (true == IsMoveKey()
		&& RunningTime_ < 0)
		// ����Ű�� �������� ����
	{
		InputDir_ = CurDir_;

		ChangeState(PlayerState::Walk);
		return;
	}
	else if (true == IsMoveKey()
		&& RunningTime_ > 0)
	{
		if (InputDir_ == CurDir_)
		{
			ChangeState(PlayerState::Run);
			return;
		}
		else
		{
			ChangeState(PlayerState::Walk);
			return;
		}

	}

	if (true == GameEngineInput::GetInst()->IsPress("Down"))
	{
		ChangeState(PlayerState::Down);
		return;
	}

	if (true == GameEngineInput::GetInst()->IsPress("Attack"))
	{
		ChangeState(PlayerState::AttackStart);
		return;
	}

	// ���� 
	if (true == IsJumpKey())
	{
		ChangeState(PlayerState::Jump);
		return;
	}

	// ������, �������� 
	HillPixelCheck();
}

void SparkKirby::WalkUpdate()
{
	RunningTime_ = 0.1f;

	if (false == IsMoveKey())
	{
		ChangeState(PlayerState::Idle);
		return;
	}

	if (true == GameEngineInput::GetInst()->IsDown("Down"))
	{
		ChangeState(PlayerState::Down);
		return;
	}

	// ���� 
	if (true == IsJumpKey())
	{
		ChangeState(PlayerState::Jump);
		return;
	}

	if (true == GameEngineInput::GetInst()->IsPress("Attack"))
	{
		ChangeState(PlayerState::AttackStart);
		return;
	}

	Move();
	StagePixelCheck(Speed_);
}

void SparkKirby::RunUpdate()
{
	if (false == IsMoveKey())
	{
		ChangeState(PlayerState::RunToStop);
		return;
	}

	// ���� 
	if (true == IsJumpKey())
	{
		ChangeState(PlayerState::Jump);
		return;
	}

	Move();
	StagePixelCheck(500.f);
}

void SparkKirby::RunToStopUpdate()
{
	// �̵� �� �̷� ��ġ �ȼ� üũ
	MoveDir += -(MoveDir * 3.f) * GameEngineTime::GetDeltaTime();
	float4 CheckPos = GetPosition() + MoveDir * GameEngineTime::GetDeltaTime() * Speed_;
	int Color = MapColImage_->GetImagePixel(CheckPos);

	if (RGB(0, 0, 0) != Color)
	{
		SetMove(MoveDir * GameEngineTime::GetDeltaTime() * Speed_);
	}

	////////////////////////////////////////////////// Stop ���� �ð�
	StopTime_ -= GameEngineTime::GetDeltaTime();

	if (StopTime_ < 0)
	{
		ChangeState(PlayerState::Idle);
		return;
	}

	// ������, �������� 
	HillPixelCheck();

}

void SparkKirby::DownUpdate()
{
	if (true == GameEngineInput::GetInst()->IsFree("Down"))
	{
		ChangeState(PlayerState::Idle);
		return;
	}


	DownTime_ -= GameEngineTime::GetDeltaTime();


	if (true == IsMoveKey()
		&& DownTime_ > 0)
	{
		ChangeState(PlayerState::Slide);
		return;
	}
}

void SparkKirby::SlideUpdate()
{
	// ����
	MoveDir += -(MoveDir * 3.f) * GameEngineTime::GetDeltaTime();

	// �� �����δ� �� ����
	{
		float4 CheckPos = GetPosition() + MoveDir * GameEngineTime::GetDeltaTime() * Speed_;

		int Color = MapColImage_->GetImagePixel(CheckPos);
		if (RGB(0, 0, 0) != Color)
		{
			SetMove(MoveDir * GameEngineTime::GetDeltaTime() * Speed_);
		}
	}

	// ����: �ִϸ��̼��� ����Ǵ� ���� �ٸ� �������� ����ȵǰ�

	/////////////////////////////////////////////////////////// �����̵� ���� �ð� 
	SlidingTime_ -= GameEngineTime::GetDeltaTime();

	if (SlidingTime_ < 0)
	{
		RunningTime_ = 0;

		ChangeState(PlayerState::Idle);
		return;
	}

	// ������, �������� 
	HillPixelCheck();
}

void SparkKirby::JumpUpdate()
{
	// ���� �̵�
	SetMove(MoveDir * GameEngineTime::GetDeltaTime());

	float4 YPos = { 0.0f, MoveDir.y };

	// ���� ���� �� ������ Pause
	if (YPos.y = -500.f)
	{
		if (20 == PlayerAnimationRender->CurrentAnimation()->WorldCurrentFrame())
		{
			PlayerAnimationRender->PauseOn();
		}
	}

	if (true == GameEngineInput::GetInst()->IsPress("MoveLeft"))
	{
		MoveDir = float4{ -300.0f, MoveDir.y };
	}
	else if (true == GameEngineInput::GetInst()->IsPress("MoveRight"))
	{
		MoveDir = float4{ 300.0f, MoveDir.y };
	}

	// Float
	YPos = { 0.0f, MoveDir.y };
	if (YPos.y >= -300.f)
	{
		if (GameEngineInput::GetInst()->IsDown("JumpRight"))
		{
			ChangeState(PlayerState::Float);
			return;
		}
		else if (GameEngineInput::GetInst()->IsDown("JumpLeft"))
		{
			ChangeState(PlayerState::Float);
			return;
		}
	}


	// �߷�
	GravityOn();

	YPos = { 0.0f, MoveDir.y };
	if (YPos.y > -500.f)
	{
		PlayerAnimationRender->PauseOff();
	}

	// �翷 + �� üũ 
	MovePixelCheck(20.0f, 20.0f);

	// �ٴڿ� ��Ҵ�
	if (RGB(0, 0, 0) == BottomPixelColorCheck(20.f))
	{
		MakeStarEffect();
		MoveDir = float4::ZERO;
		ChangeState(PlayerState::Idle);
		return;
	}
	//else if (RGB(0, 0, 0) != BottomPixelColorCheck(400.f))
	//{
	//	ChangeState(PlayerState::Fall);
	//	return;
	//}
}

void SparkKirby::FloatUpdate()
{
	// ���� ����� ��������
	if (GameEngineInput::GetInst()->IsDown("Inhale"))
	{
		if (true == IsJumpKey())
		{
			ChangeState(PlayerState::Float);
			return;
		}

		FloatEffSound_.Stop();
		ChangeState(PlayerState::Exhale);
		return;
	}

	if (true == PlayerAnimationRender->IsEndAnimation())
	{
		PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_ + "_Loop");
	}

	// Float���¿��� �̵�
	//MoveDir = float4::ZERO;

	if (true == GameEngineInput::GetInst()->IsPress("MoveLeft"))
	{
		MoveDir = float4{ -200.f, MoveDir.y };
		PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_ + "_Loop");
	}
	else if (true == GameEngineInput::GetInst()->IsPress("MoveRight"))
	{
		MoveDir = float4{ 200.f, MoveDir.y };
		PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_ + "_Loop");
	}
	else if (false == IsMoveKey())
	{
		MoveDir.x = 0.0f;
	}

	// �߷�
	if (false == IsJumpKey())
	{
		MoveDir.y = 100.f;
		SetMove(MoveDir * GameEngineTime::GetDeltaTime());
	}
	else if (true == IsJumpKey())
	{
		MoveDir.y = -200.f;
		SetMove(MoveDir * GameEngineTime::GetDeltaTime());
	}

	// �� �� + �� �ȼ� üũ
	MovePixelCheck(20.0f, 20.0f);

	// Float���·� �ٴڿ� ��Ҵ�
	if (RGB(0, 0, 0) == BottomPixelColorCheck(20.f))
	{
		MoveDir = float4::UP;
		SetMove(MoveDir);
	}
}

void SparkKirby::FallUpdate()
{
	// ���߿� JumpŰ ������ Float���� ��ȯ 
	if (GameEngineInput::GetInst()->IsPress("JumpLeft"))
	{
		ChangeState(PlayerState::Float);
		return;
	}
	else if (GameEngineInput::GetInst()->IsPress("JumpRight"))
	{
		ChangeState(PlayerState::Float);
		return;
	}

	// ����Ű�� ������ �ش� �������� x �̵�
	if (true == GameEngineInput::GetInst()->IsPress("MoveLeft"))
	{
		MoveDir = float4{ -200.0f, MoveDir.y };
	}
	else if (true == GameEngineInput::GetInst()->IsPress("MoveRight"))
	{
		MoveDir = float4{ 200.0f, MoveDir.y };
	}

	// MoveDir.x�� �������� �ʰ� y�� �����Ѵ� 
	MoveDir.y += 1500.f * GameEngineTime::GetDeltaTime();


	// ���� ���� �ʾҴٸ� MoveDir.y�� �����ϸ� �������� 
	if (RGB(0, 0, 0) != BottomPixelColorCheck(20.f))
	{
		SetMove(MoveDir * GameEngineTime::GetDeltaTime());
	}
	else
	{
		MakeStarEffect();
		ChangeState(PlayerState::FallToBounce);
		return;
	}

}

void SparkKirby::FallToBounceUpdate()
{
	// ����Ű�� ������ �ش� �������� x �̵�
	if (true == GameEngineInput::GetInst()->IsPress("MoveLeft"))
	{
		MoveDir = float4{ -300.0f, MoveDir.y };
	}
	else if (true == GameEngineInput::GetInst()->IsPress("MoveRight"))
	{
		MoveDir = float4{ 300.0f, MoveDir.y };
	}

	// ���� ������ ���� �� �� ƨ���
	if (RGB(0, 0, 0) == BottomPixelColorCheck(20.f))
	{
		MoveDir.y = -400.f;
	}

	SetMove(MoveDir * GameEngineTime::GetDeltaTime());

	// ƨ������ BounceToIdle�� ��ȯ
	if (RGB(0, 0, 0) != BottomPixelColorCheck(130.f))
	{
		ChangeState(PlayerState::BounceToIdle);
		return;
	}
}

void SparkKirby::BounceToIdleUpdate()
{
	// ����Ű�� ������ �ش� �������� x �̵�
	if (true == GameEngineInput::GetInst()->IsPress("MoveLeft"))
	{
		MoveDir = float4{ -300.0f, MoveDir.y };
	}
	else if (true == GameEngineInput::GetInst()->IsPress("MoveRight"))
	{
		MoveDir = float4{ 300.0f, MoveDir.y };
	}

	// MoveDir.x�� �������� �ʰ� y�� �����Ѵ� 
	MoveDir.y += 1300.f * GameEngineTime::GetDeltaTime();
	SetMove(MoveDir * GameEngineTime::GetDeltaTime());

	// ���� ������ Idle�� ��ȯ
	if (RGB(0, 0, 0) == BottomPixelColorCheck(20.f))
	{
		MoveDir = float4::ZERO;

		MakeStarEffect();
		ChangeState(PlayerState::Idle);
		return;
	}

}

void SparkKirby::ExhaleUpdate()
{
	if (PlayerAnimationRender->IsEndAnimation())
	{
		// ���̶� �ʹ� ������ -> Idle
		if (RGB(0, 0, 0) == BottomPixelColorCheck(150.f))
		{
			MoveDir = float4::ZERO;

			MakeStarEffect();
			ChangeState(PlayerState::Idle);
			return;
		}


		// ������ ������ �Ÿ� ������ ������ Fall�� ��ȯ
		MoveDir.y += 900.f * GameEngineTime::GetDeltaTime();
		SetMove(MoveDir * GameEngineTime::GetDeltaTime());

		ChangeState(PlayerState::Fall);
		return;
	}
}

void SparkKirby::AttackStartUpdate()
{
	if (PlayerAnimationRender->IsEndAnimation())
	{
		SparkAttackCollision_->On();

		ChangeState(PlayerState::Attack);
		return;
	}
}

void SparkKirby::AttackUpdate()
{
	if (true == GameEngineInput::GetInst()->IsFree("Attack"))
	{
		AttackEffSound_.Stop();
		AttackEffSoundMid_.Stop();

		SparkAttackCollision_->Off();

		ChangeState(PlayerState::AttackEnd);
		return;
	}
}

void SparkKirby::AttackEndUpdate()
{
	if (PlayerAnimationRender->IsEndAnimation())
	{
		ChangeState(PlayerState::Idle);
		return;
	}
}

void SparkKirby::IdleStart()
{
	MoveDir = float4::ZERO;;

	Speed_ = 350.f;

	AnimationName_ = "Idle_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void SparkKirby::WalkStart()
{
	Speed_ = 350.f;

	AnimationName_ = "Walk_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void SparkKirby::RunStart()
{
	{
		Effect_Slide* Effect = GetLevel()->CreateActor<Effect_Slide>((int)ORDER::EFFECT);

		if (CurDir_ == PlayerDir::Right)
		{
			Effect->SetPosition(GetPosition() + float4{ -40.f, 40.f });
			Effect->SetDir(EffectDir::Right);

		}
		else if (CurDir_ == PlayerDir::Left)
		{
			Effect->SetPosition(GetPosition() + float4{ 40.f, 40.f });
			Effect->SetDir(EffectDir::Left);
		}
	}
	GameEngineSound::SoundPlayOneShot("Slide.wav");

	Speed_ = 500.f;

	AnimationName_ = "Run_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void SparkKirby::RunToStopStart()
{
	{
		Effect_RunToStop* Effect = GetLevel()->CreateActor<Effect_RunToStop>((int)ORDER::EFFECT);

		if (CurDir_ == PlayerDir::Right)
		{
			Effect->SetPosition(GetPosition() + float4{ -40.f, 50.f });
			Effect->SetDir(EffectDir::Right);

		}
		else if (CurDir_ == PlayerDir::Left)
		{
			Effect->SetPosition(GetPosition() + float4{ 40.f, 50.f });
			Effect->SetDir(EffectDir::Left);
		}
	}
	GameEngineSound::SoundPlayOneShot("RunToStop.wav");

	Speed_ = 350.f;
	StopTime_ = 0.3f;

	AnimationName_ = "RunToStop_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void SparkKirby::DownStart()
{
	DownTime_ = 0.5f;
	AnimationName_ = "Down_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);

}

void SparkKirby::SlideStart()
{
	{
		Effect_Slide* Effect = GetLevel()->CreateActor<Effect_Slide>((int)ORDER::EFFECT);

		if (CurDir_ == PlayerDir::Right)
		{
			Effect->SetPosition(GetPosition() + float4{ -45.f, 40.f });
			Effect->SetDir(EffectDir::Right);

		}
		else if (CurDir_ == PlayerDir::Left)
		{
			Effect->SetPosition(GetPosition() + float4{ 45.f, 40.f });
			Effect->SetDir(EffectDir::Left);
		}
	}
	GameEngineSound::SoundPlayOneShot("Slide.wav");

	Speed_ = 500.f;
	SlidingTime_ = 1.2f;

	if (true == GameEngineInput::GetInst()->IsDown("MoveLeft"))
	{
		MoveDir = float4::LEFT;

	}
	else if (true == GameEngineInput::GetInst()->IsDown("MoveRight"))
	{
		MoveDir = float4::RIGHT;
	}

	AnimationName_ = "Slide_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);

}

void SparkKirby::JumpStart()
{
	GameEngineSound::SoundPlayOneShot("Jump.wav");

	//FallTime_ = 0.8f;
	JumpPower_ = 1000.f;
	Gravity_ = 1800.f;

	// �� ���� 100�� ������ ���� ���� 
	MoveDir = float4::UP * JumpPower_;

	AnimationName_ = "Jump_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void SparkKirby::FloatStart()
{
	FloatEffSound_.Stop();
	FloatEffSound_ = GameEngineSound::SoundPlayControl("Float.wav");
	//FallTime_ = 0.8f;
	Speed_ = 3.f;
	Gravity_ = 300.f;

	PlayerAnimationRender->PauseOff();

	AnimationName_ = "Float_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);

}

void SparkKirby::FallStart()
{
	MoveDir = float4::ZERO;;

	AnimationName_ = "Fall_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void SparkKirby::FallToBounceStart()
{
	GameEngineSound::SoundPlayOneShot("Jump.wav");

	Gravity_ = 0.0f;

	MoveDir = float4::ZERO;

	AnimationName_ = "FallToBounce_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void SparkKirby::BounceToIdleStart()
{
	MoveDir = float4::ZERO;

	AnimationName_ = "BounceToIdle_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void SparkKirby::ExhaleStart()
{
	{
		Effect_Exhale* Effect = GetLevel()->CreateActor<Effect_Exhale>((int)ORDER::EFFECT);

		if (CurDir_ == PlayerDir::Right)
		{
			Effect->SetPosition(GetPosition() + float4{ 80.f, 55.f });
			Effect->SetDir(EffectDir::Right);

		}
		else if (CurDir_ == PlayerDir::Left)
		{
			Effect->SetPosition(GetPosition() + float4{ -80.f, 55.f });
			Effect->SetDir(EffectDir::Left);
		}
	}
	GameEngineSound::SoundPlayOneShot("Exhale.wav");

	AnimationName_ = "Exhale_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void SparkKirby::AttackStartStart()
{
	GameEngineSound::SoundPlayOneShot("Spark3.wav");

	AnimationName_ = "AttackStart_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void SparkKirby::AttackStart()
{
	AttackEffSound_.Stop();
	AttackEffSoundMid_.Stop();
	AttackEffSound_ = GameEngineSound::SoundPlayControl("Spark2.wav");
	AttackEffSoundMid_ = GameEngineSound::SoundPlayControl("Spark3.wav");

	AnimationName_ = "Attack_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void SparkKirby::AttackEndStart()
{
	GameEngineSound::SoundPlayOneShot("Spark.wav");

	AnimationName_ = "AttackStart_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

