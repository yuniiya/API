#include "IceKirby.h"
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
#include "Effect_IceBreath.h"
#include "Effect_IceBox.h"
#include "Effect_Exhale.h"
#include "Effect_ReleaseSkill.h"


IceKirby* IceKirby::IcePlayer = nullptr;
SkillUI* IceKirby::IceSkill = nullptr;
SkillName* IceKirby::IceName = nullptr;

IceKirby::IceKirby()
	: CurSkill_(KirbySkill::Ice)
	, Speed_(350.0f)
	, JumpPower_(1000.f)
	, Gravity_(1500.f)
	, StopTime_(1.f)
	, DownTime_(0.5f)
	, SlidingTime_(1.2f)
	, PlayerAnimationRender(nullptr)
{

}
IceKirby::~IceKirby()
{

}
void IceKirby::MonsterColCheck()
{
	std::vector<GameEngineCollision*> ColList;

	if (true == PlayerCollision->CollisionResult("DefaultMonster", ColList, CollisionType::Rect, CollisionType::Rect))
	{
	/*	Off();
		IceSkill->Off();
		IceName->Off();

		MainPlayer->SetPosition(GetPosition());
		CurSkill_ = KirbySkill::Default;
		MainPlayer->On();*/
	}

}

void IceKirby::LevelChangeStart(GameEngineLevel* _PrevLevel)
{
	IcePlayer = this;
}

void IceKirby::Start()
{
	//SetScale({ 1000, 1000 });

	PlayerCollision = CreateCollision("PlayerHitBox", { 70, 70 });

	// �ִϸ��̼��� �ϳ��� ����� �ִϸ��̼��� ����ȴ�.
	PlayerAnimationRender = CreateRenderer();
	PlayerAnimationRender->SetPivotType(RenderPivot::BOT);
	PlayerAnimationRender->SetPivot({ -4.f, 250.f });

	// Walk_Right�̹����� 0~9�ε����� 0.1�ʵ��� ��� (true = ����on)
	//Render->SetPivotType(RenderPivot::BOT);

	// Left
	{
		PlayerAnimationRender->CreateAnimation("Ice_Left.bmp", "Idle_Left", 0, 1, 1.f, true);
		PlayerAnimationRender->CreateAnimation("Ice_Left.bmp", "Down_Left", 2, 3, 1.f, true);
		PlayerAnimationRender->CreateAnimation("Ice_Left.bmp", "Slide_Left", 4, 4, 0.3f, false);
		PlayerAnimationRender->CreateAnimation("Ice_Left.bmp", "Jump_Left", 6, 14, 0.03f, false);
		PlayerAnimationRender->CreateAnimation("Ice_Left.bmp", "Walk_Left", 15, 24, 0.07f, true);
		PlayerAnimationRender->CreateAnimation("Ice_Left.bmp", "Run_Left", 25, 32, 0.07f, true);
		PlayerAnimationRender->CreateAnimation("Ice_Left.bmp", "RunToStop_Left", 33, 33, 0.3f, false);

		// Float
		PlayerAnimationRender->CreateAnimation("Ice_Left.bmp", "Float_Left", 34, 38, 0.05f, true);
		PlayerAnimationRender->CreateAnimation("Ice_Left.bmp", "Float_Left_Loop", 39, 54, 0.08f, true);

		PlayerAnimationRender->CreateAnimation("Ice_Left.bmp", "Exhale_Left", 57, 57, 0.1f, false);

		// Fall
		PlayerAnimationRender->CreateAnimation("Ice_Left.bmp", "Fall_Left", 59, 63, 0.2f, false);
		PlayerAnimationRender->CreateAnimation("Ice_Left.bmp", "FallToBounce_Left", 64, 68, 0.02f, false);
		PlayerAnimationRender->CreateAnimation("Ice_Left.bmp", "BounceToIdle_Left", 69, 72, 0.2f, false);

		// Attack
		//PlayerAnimationRender->CreateAnimation("Default_Attack_Left.bmp", "AttackStart_Left", 0, 1, 0.05f, true);
		PlayerAnimationRender->CreateAnimation("Ice_Left.bmp", "AttackStart_Left", 93, 98, 0.06f, false);
		PlayerAnimationRender->CreateAnimation("Ice_Left.bmp", "Attack_Left", 99, 100, 0.07f, true);
		PlayerAnimationRender->CreateAnimation("Ice_Left.bmp", "AttackEnd_Left", 101, 102, 0.05f, false);
	}


	// Right
	{
		PlayerAnimationRender->CreateAnimation("Ice_Right.bmp", "Idle_Right", 0, 1, 1.f, true);
		PlayerAnimationRender->CreateAnimation("Ice_Right.bmp", "Down_Right", 2, 3, 1.f, true);
		PlayerAnimationRender->CreateAnimation("Ice_Right.bmp", "Slide_Right", 4, 4, 0.3f, false);
		PlayerAnimationRender->CreateAnimation("Ice_Right.bmp", "Jump_Right", 6, 14, 0.03f, false);
		PlayerAnimationRender->CreateAnimation("Ice_Right.bmp", "Walk_Right", 15, 24, 0.07f, true);
		PlayerAnimationRender->CreateAnimation("Ice_Right.bmp", "Run_Right", 25, 32, 0.07f, true);
		PlayerAnimationRender->CreateAnimation("Ice_Right.bmp", "RunToStop_Right", 33, 33, 0.3f, false);

		// Float
		PlayerAnimationRender->CreateAnimation("Ice_Right.bmp", "Float_Right", 34, 38, 0.05f, true);
		PlayerAnimationRender->CreateAnimation("Ice_Right.bmp", "Float_Right_Loop", 39, 54, 0.08f, true);

		PlayerAnimationRender->CreateAnimation("Ice_Right.bmp", "Exhale_Right", 57, 57, 0.1f, false);

		// Fall
		PlayerAnimationRender->CreateAnimation("Ice_Right.bmp", "Fall_Right", 59, 63, 0.2f, false);
		PlayerAnimationRender->CreateAnimation("Ice_Right.bmp", "FallToBounce_Right", 64, 68, 0.02f, false);
		PlayerAnimationRender->CreateAnimation("Ice_Right.bmp", "BounceToIdle_Right", 69, 72, 0.2f, false);

		// Attack
		//PlayerAnimationRender->CreateAnimation("Default_Attack_Left.bmp", "AttackStart_Left", 0, 1, 0.05f, true);
		PlayerAnimationRender->CreateAnimation("Ice_Right.bmp", "AttackStart_Right", 93, 98, 0.06f, false);
		PlayerAnimationRender->CreateAnimation("Ice_Right.bmp", "Attack_Right", 99, 100, 0.07f, true);
		PlayerAnimationRender->CreateAnimation("Ice_Right.bmp", "AttackEnd_Right", 101, 102, 0.05f, false);
	}
	
	AnimationName_ = "Idle_";
	PlayerAnimationRender->ChangeAnimation("Idle_Right");

	Off();
	IcePlayer = this;

	IceSkill = GetLevel()->CreateActor<SkillUI>((int)ORDER::UI);
	IceSkill->GetRenderer()->SetImage("Icon_Ice.bmp");
	IceSkill->SetPosition({ 67.f, 665.f });
	IceSkill->Off();

	IceName = GetLevel()->CreateActor<SkillName>((int)ORDER::NAMEUI);
	IceName->GetRenderer()->SetImage("UI_Ice.bmp");
	IceName->Off();
}

void IceKirby::Update()
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

	if (true == GameEngineInput::GetInst()->IsDown("SkillRelease"))
	{
		// ��ų ���� ����
		GameEngineSound::SoundPlayOneShot("Release1.wav");

		{
			Effect_ReleaseSkill* Effect = GetLevel()->CreateActor<Effect_ReleaseSkill>((int)ORDER::EFFECT);

			if (CurDir_ == PlayerDir::Right)
			{
				Effect->SetPosition(GetPosition());
				Effect->SetDir(EffectDir::Right);

			}
			else if (CurDir_ == PlayerDir::Left)
			{
				Effect->SetPosition(GetPosition());
				Effect->SetDir(EffectDir::Left);
			}
		}

		Off();
		IceSkill->Off();
		IceName->Off();

		// ����Ʈ Ŀ�� On
		MainPlayer->SetPosition(GetPosition());
		CurSkill_ = KirbySkill::Default;
		MainPlayer->On();

		MainPlayer->InhaleCollision->Off();
	}
}

void IceKirby::ChangeState(PlayerState _State)
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
		case PlayerState::Down:
			DownStart();
			break;
		case PlayerState::Run:
			RunStart();
			break;
		case PlayerState::RunToStop:
			RunToStopStart();
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
		case PlayerState::Exhausted:
			ExhaustedStart();
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

void IceKirby::PlayerStateUpdate()
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

void IceKirby::DirAnimationCheck()
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

void IceKirby::IdleUpdate()
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

void IceKirby::WalkUpdate()
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

void IceKirby::RunUpdate()
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

void IceKirby::RunToStopUpdate()
{
	// �̵� �� �̷� ��ġ �ȼ� üũ
	MoveDir.x += -(MoveDir.x * 3.f) * GameEngineTime::GetDeltaTime();

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

	HillPixelCheck();

}

void IceKirby::DownUpdate()
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

void IceKirby::SlideUpdate()
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

void IceKirby::JumpUpdate()
{
	// ���� �̵�
	SetMove(MoveDir * GameEngineTime::GetDeltaTime());

	float4 YPos = { 0.0f, MoveDir.y };

	// ���� ���� �� ������ Pause
	if (YPos.y = -500.f)
	{
		if (6 == PlayerAnimationRender->CurrentAnimation()->WorldCurrentFrame())
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

void IceKirby::FloatUpdate()
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

		MonsterColCheck();
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

void IceKirby::FallUpdate()
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

void IceKirby::FallToBounceUpdate()
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

void IceKirby::BounceToIdleUpdate()
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

void IceKirby::ExhaleUpdate()
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


void IceKirby::AttackStartUpdate()
{
	if (PlayerAnimationRender->IsEndAnimation())
	{
		ChangeState(PlayerState::Attack);
		return;
	}
}

void IceKirby::AttackUpdate()
{
	if (true == GameEngineInput::GetInst()->IsFree("Attack"))
	{
		AttackEffSound_.Stop();
		ChangeState(PlayerState::AttackEnd);
		return;
	}
}

void IceKirby::AttackEndUpdate()
{
	if (PlayerAnimationRender->IsEndAnimation())
	{
		IceBreath_->Death();
		ChangeState(PlayerState::Idle);
		return;
	}
}


void IceKirby::IdleStart()
{
	MoveDir = float4::ZERO;;

	Speed_ = 350.f;

	AnimationName_ = "Idle_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void IceKirby::WalkStart()
{
	Speed_ = 350.f;

	AnimationName_ = "Walk_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void IceKirby::RunStart()
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

void IceKirby::RunToStopStart()
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

void IceKirby::DownStart()
{
	DownTime_ = 0.5f;
	AnimationName_ = "Down_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);

}

void IceKirby::SlideStart()
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

void IceKirby::JumpStart()
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

void IceKirby::FloatStart()
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

void IceKirby::FallStart()
{
	MoveDir = float4::ZERO;;

	AnimationName_ = "Fall_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void IceKirby::FallToBounceStart()
{
	GameEngineSound::SoundPlayOneShot("Jump.wav");

	Gravity_ = 0.0f;

	MoveDir = float4::ZERO;

	AnimationName_ = "FallToBounce_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void IceKirby::BounceToIdleStart()
{
	MoveDir = float4::ZERO;

	AnimationName_ = "BounceToIdle_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void IceKirby::ExhaleStart()
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

void IceKirby::AttackStartStart()
{
	AnimationName_ = "AttackStart_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void IceKirby::AttackStart()
{
	{
		IceBreath_ = GetLevel()->CreateActor<Effect_IceBreath>((int)ORDER::EFFECT);

		if (CurDir_ == PlayerDir::Right)
		{
			IceBreath_->SetPosition(GetPosition() + float4{ 155.f, 10.f });
			IceBreath_->SetDir(EffectDir::Right);

		}
		else if (CurDir_ == PlayerDir::Left)
		{
			IceBreath_->SetPosition(GetPosition() + float4{ -155.f, 10.f });
			IceBreath_->SetDir(EffectDir::Left);
		}
	}
	
	AttackEffSound_.Stop();
	AttackEffSound_ = GameEngineSound::SoundPlayControl("IceBreath3.wav");

	AnimationName_ = "Attack_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void IceKirby::AttackEndStart()
{
	AnimationName_ = "AttackEnd_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}




