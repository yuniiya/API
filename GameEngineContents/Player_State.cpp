#include "Player.h"
#include <GameEngine/GameEngine.h>
#include <GameEngineBase/GameEngineWindow.h>
#include <GameEngine/GameEngineImageManager.h>
#include <GameEngineBase/GameEngineInput.h>
#include <GameEngineBase/GameEngineTime.h>
#include <GameEngine/GameEngineRenderer.h>
#include <GameEngine/GameEngineCollision.h>

#include <GameEngine/GameEngineLevel.h> // ������ ���ؼ�
#include "Bullet.h" // �Ѿ��� ����� �ʹ�.

void Player::IdleUpdate()
{
	MonsterColCheck();
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

	if (true == GameEngineInput::GetInst()->IsPress("Inhale"))
	{
		ChangeState(PlayerState::Inhale);
		return;
	}

	// ���� 
	if (true == IsJumpKey())
	{
		ChangeState(PlayerState::Jump);
		return;
	}

	// ������, �������� 
	float4 RightDownkPos = GetPosition() + float4{ 0.f,20.f };
	float4 LeftUpPos = GetPosition() + float4{ -20.f,0.f };

	int DownColor = MapColImage_->GetImagePixel(RightDownkPos);
	int UpColor = MapColImage_->GetImagePixel(LeftUpPos);


	float4 XMove = { MoveDir.x, 0.0f };
	float4 YMove = { 0.0f, MoveDir.y };

	if (RGB(0, 0, 0) != DownColor)
	{
		SetMove(float4::DOWN);
	}
	else if (RGB(0, 0, 0) != UpColor)
	{
		SetMove(YMove);
	}

	
}

void Player::WalkUpdate()
{
	MonsterColCheck();
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

	if (true == GameEngineInput::GetInst()->IsPress("Inhale"))
	{
		ChangeState(PlayerState::Inhale);
		return;
	}

	Move();
	StagePixelCheck(Speed_);

	// ������, �������� 
	//HillPixelCheck();
	float4 CheckPos = float4::DOWN;
	float4 LeftUpPos = float4::UP;
	float4 RightUpPos = float4::UP;

	int DownColor = MapColImage_->GetImagePixel(GetPosition() + float4{ 0.0f, 20.0f } + CheckPos);
	int LeftColor = MapColImage_->GetImagePixel(GetPosition() + float4{ -20.0f, 0.0f } + LeftUpPos);
	int RightColor = MapColImage_->GetImagePixel(GetPosition() + float4{ 20.0f, 0.0f } + RightUpPos);

	if (RGB(0, 0, 0) != DownColor)
	{
		// ���� ����ִ� ������ ��� �����ش�
		while (RGB(0, 0, 0) == DownColor)
		{
			CheckPos += float4::DOWN;
			DownColor = MapColImage_->GetImagePixel(GetPosition() + CheckPos);
		}
		SetMove(CheckPos);
	}
	else if (RGB(0, 0, 0) == LeftColor)
	{
		while (RGB(0, 0, 0) != LeftColor)
		{
			LeftUpPos += float4::UP;
			LeftColor = MapColImage_->GetImagePixel(GetPosition() + LeftUpPos);
		}
		SetMove(LeftUpPos);
	}
	else if (RGB(0, 0, 0) == RightColor)
	{
		while (RGB(0, 0, 0) != RightColor)
		{
			RightUpPos += float4::UP;
			RightColor = MapColImage_->GetImagePixel(GetPosition() + RightUpPos);
		}
		SetMove(RightUpPos);
	}
}

void Player::RunUpdate()
{
	MonsterColCheck();
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



	// ������, �������� 
	//HillPixelCheck();
	float4 CheckPos = float4::DOWN;
	float4 LeftUpPos = float4::UP;
	float4 RightUpPos = float4::UP;

	int DownColor = MapColImage_->GetImagePixel(GetPosition() + float4{ 0.0f, 20.0f } + CheckPos);
	int LeftColor = MapColImage_->GetImagePixel(GetPosition() + float4{ -20.0f, 0.0f } + LeftUpPos);
	int RightColor = MapColImage_->GetImagePixel(GetPosition() + float4{ 20.0f, 0.0f } + RightUpPos);

	if (RGB(0, 0, 0) != DownColor)
	{
		// ���� ����ִ� ������ ��� �����ش�
		while (RGB(0, 0, 0) == DownColor)
		{
			CheckPos += float4::DOWN;
			DownColor = MapColImage_->GetImagePixel(GetPosition() + CheckPos);
		}
		SetMove(CheckPos);
	}
	else if (RGB(0, 0, 0) == LeftColor)
	{
		while (RGB(0, 0, 0) != LeftColor)
		{
			LeftUpPos += float4::UP;
			LeftColor = MapColImage_->GetImagePixel(GetPosition() + LeftUpPos);
		}
		SetMove(LeftUpPos);
	}
	else if (RGB(0, 0, 0) == RightColor)
	{
		while (RGB(0, 0, 0) != RightColor)
		{
			RightUpPos += float4::UP;
			RightColor = MapColImage_->GetImagePixel(GetPosition() + RightUpPos);
		}
		SetMove(RightUpPos);
	}

	// �ӷ� ����
	//if (1.f <= MoveDir.Len2D())
	//{
	//	MoveDir.Range2D(1.f);
	//}

	// �ƹ��� Ű�� ������ ������ ���� �����Ѵ�
	//if (false == IsMoveKey())
	//{
	//	MoveDir += -MoveDir * GameEngineTime::GetDeltaTime();

	//	if (0.005f >= MoveDir.Len2D())
	//	{
	//		MoveDir = float4::ZERO;
	//		return;
	//	}

}

void Player::RunToStopUpdate()
{
	MonsterColCheck();
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

void Player::DownUpdate()
{
	MonsterColCheck();
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

void Player::SlideUpdate()
{
	// ����
	MoveDir += -(MoveDir * 3.f) * GameEngineTime::GetDeltaTime();

	// �� �����δ� �� ����
	StagePixelCheck(Speed_);

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
	//HillPixelCheck();
	float4 CheckPos = float4::DOWN;
	float4 LeftUpPos = float4::UP;
	float4 RightUpPos = float4::UP;

	int DownColor = MapColImage_->GetImagePixel(GetPosition() + float4{ 0.0f, 20.0f } + CheckPos);
	int LeftColor = MapColImage_->GetImagePixel(GetPosition() + float4{ -20.0f, 0.0f } + LeftUpPos);
	int RightColor = MapColImage_->GetImagePixel(GetPosition() + float4{ 20.0f, 0.0f } + RightUpPos);

	if (RGB(0, 0, 0) != DownColor)
	{
		// ���� ����ִ� ������ ��� �����ش�
		while (RGB(0, 0, 0) == DownColor)
		{
			CheckPos += float4::DOWN;
			DownColor = MapColImage_->GetImagePixel(GetPosition() + CheckPos);
		}
		SetMove(CheckPos);
	}
	else if (RGB(0, 0, 0) == LeftColor)
	{
		while (RGB(0, 0, 0) != LeftColor)
		{
			LeftUpPos += float4::UP;
			LeftColor = MapColImage_->GetImagePixel(GetPosition() + LeftUpPos);
		}
		SetMove(LeftUpPos);
	}
	else if (RGB(0, 0, 0) == RightColor)
	{
		while (RGB(0, 0, 0) != RightColor)
		{
			RightUpPos += float4::UP;
			RightColor = MapColImage_->GetImagePixel(GetPosition() + RightUpPos);
		}
		SetMove(RightUpPos);
	}
}

void Player::JumpUpdate()
{
	MonsterColCheck();
	// ���� �̵�
	SetMove(MoveDir * GameEngineTime::GetDeltaTime());

	float4 YPos = { 0.0f, MoveDir.y };

	// ���� ���� �� ������ Pause
	if (YPos.y = -500.f)
	{
		if (0 == PlayerAnimationRender->CurrentAnimation()->WorldCurrentFrame())
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

void Player::FloatUpdate()
{
	MonsterColCheck();
	// ���� ����� ��������
	if (GameEngineInput::GetInst()->IsDown("Inhale"))
	{
		if (true == IsJumpKey())
		{
			ChangeState(PlayerState::Float);
			return;
		}

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
		MoveDir = float4{-200.f, MoveDir.y};
		PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_ + "_Loop");
	}
	else if (true == GameEngineInput::GetInst()->IsPress("MoveRight"))
	{
		MoveDir = float4{200.f, MoveDir.y };
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

void Player::FallUpdate()
{
	MonsterColCheck();
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
		ChangeState(PlayerState::FallToBounce);
		return;
	}

}

void Player::FallToBounceUpdate()
{
	MonsterColCheck();
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

void Player::BounceToIdleUpdate()
{
	MonsterColCheck();
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

		ChangeState(PlayerState::Idle);
		return;
	}
	
}

void Player::InhaleUpdate()
{
	if (true == GameEngineInput::GetInst()->IsPress("Inhale"))
	{
		if (CurDir_ == PlayerDir::Left)
		{
			InhalePos.x = -90.f;
			
		}
		else
		{
			InhalePos.x = 90.f;
		}

		InhaleCollision->SetPivot({ InhalePos });
		InhaleCollision->On();

		InhaleColCheck();
	}


	InhaleTime_ -= GameEngineTime::GetDeltaTime();

	if (PlayerAnimationRender->IsEndAnimation())
	{
		PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_ + "_Loop");
	}
	
	// 2.5�� �� Exhausted
	if (InhaleTime_ <= 0)
	{
		InhaleCollision->Off();
		ChangeState(PlayerState::Exhausted);
		return;
	}

	// Ű���� �� ���� �� -> Idle
	if(GameEngineInput::GetInst()->IsUp("Inhale"))
	{
		InhaleCollision->Off();
		ChangeState(PlayerState::Idle);
		return;
	}

	// �ȱ�
	if (true == IsMoveKey())
	{
		InhaleCollision->Off();
		ChangeState(PlayerState::Walk);
		return;
	}

	// ����
	if (true == IsJumpKey())
	{
		InhaleCollision->Off();
		ChangeState(PlayerState::Jump);
		return;
	}
}

void Player::FullUpdate()
{
	float Time = 0.0f;

	if (true == GameEngineInput::GetInst()->IsPress("Down"))
	{
		ChangeState(PlayerState::Swallow);
		return;
	}

	if (true == IsMoveKey())
	{
		ChangeState(PlayerState::FullWalk);
		return;
	}

	if (true == IsJumpKey())
	{
		ChangeState(PlayerState::FullJump);
		return;
	}

	
	//Time += GameEngineTime::GetDeltaTime();
	//if (1.f <= Time)
	//{
	//	if (true == GameEngineInput::GetInst()->IsPress("Inhale"))
	//	{
	//		ChangeState(PlayerState::AttackStart);
	//		return;
	//	}
	//}
	//
}


void Player::FullWalkUpdate()
{
	MonsterColCheck();
	RunningTime_ = 0.1f;

	if (false == IsMoveKey())
	{
		ChangeState(PlayerState::Full);
		return;
	}

	// ���� 
	if (true == IsJumpKey())
	{
		ChangeState(PlayerState::FullJump);
		return;
	}

	if (true == GameEngineInput::GetInst()->IsPress("Inhale"))
	{
		ChangeState(PlayerState::AttackStart);
		return;
	}

	Move();
	StagePixelCheck(Speed_);

	// ������, �������� 
	//HillPixelCheck();
	float4 CheckPos = float4::DOWN;
	float4 LeftUpPos = float4::UP;
	float4 RightUpPos = float4::UP;

	int DownColor = MapColImage_->GetImagePixel(GetPosition() + float4{ 0.0f, 20.0f } + CheckPos);
	int LeftColor = MapColImage_->GetImagePixel(GetPosition() + float4{ -20.0f, 0.0f } + LeftUpPos);
	int RightColor = MapColImage_->GetImagePixel(GetPosition() + float4{ 20.0f, 0.0f } + RightUpPos);

	if (RGB(0, 0, 0) != DownColor)
	{
		// ���� ����ִ� ������ ��� �����ش�
		while (RGB(0, 0, 0) == DownColor)
		{
			CheckPos += float4::DOWN;
			DownColor = MapColImage_->GetImagePixel(GetPosition() + CheckPos);
		}
		SetMove(CheckPos);
	}
	else if (RGB(0, 0, 0) == LeftColor)
	{
		while (RGB(0, 0, 0) != LeftColor)
		{
			LeftUpPos += float4::UP;
			LeftColor = MapColImage_->GetImagePixel(GetPosition() + LeftUpPos);
		}
		SetMove(LeftUpPos);
	}
	else if (RGB(0, 0, 0) == RightColor)
	{
		while (RGB(0, 0, 0) != RightColor)
		{
			RightUpPos += float4::UP;
			RightColor = MapColImage_->GetImagePixel(GetPosition() + RightUpPos);
		}
		SetMove(RightUpPos);
	}
}

void Player::FullJumpUpdate()
{
	MonsterColCheck();
}

void Player::ExhaleUpdate()
{
	MonsterColCheck();
	if (PlayerAnimationRender->IsEndAnimation())
	{
		// ���̶� �ʹ� ������ -> Idle
		if (RGB(0, 0, 0) == BottomPixelColorCheck(150.f))
		{
			MoveDir = float4::ZERO;

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

void Player::SwallowUpdate()
{
	if (PlayerAnimationRender->IsEndAnimation())
	{
		ChangeState(PlayerState::Idle);
		return;
	}
}

void Player::ExhaustedUpdate()
{
	MonsterColCheck();
	if (PlayerAnimationRender->IsEndAnimation())
	{
		ChangeState(PlayerState::Idle);
		return;
	}
}

void Player::AttackStartUpdate()
{
	if (PlayerAnimationRender->IsEndAnimation())
	{
		ChangeState(PlayerState::Attack);
		return;
	}
}

void Player::AttackUpdate()
{
	if (PlayerAnimationRender->IsEndAnimation())
	{
		ChangeState(PlayerState::AttackEnd);
		return;
	}
}

void Player::AttackEndUpdate()
{
	if (PlayerAnimationRender->IsEndAnimation())
	{
		ChangeState(PlayerState::Idle);
		return;
	}
}

void Player::DamagedStartUpdate()
{
	//PlayerAnimationRender->PauseOn();

	//float Time = 0.0f;

	//Time += GameEngineTime::GetDeltaTime();

	//if (1.f >= Time)
	//{
	//	ChangeState(PlayerState::Damaged);
	//	return;
	//}

	if (CurDir_ == PlayerDir::Right)
	{
		MoveDir.x = -0.5f;
	}
	else if (CurDir_ == PlayerDir::Left)
	{
		MoveDir.x = 0.5f;
	}

	SetMove(MoveDir);

	if (PlayerAnimationRender->IsEndAnimation())
	{
		ChangeState(PlayerState::Damaged);
		return;
	}


	//if (1.8f <= GetAccTime())
	//{
	//	ReSetAccTime();

	//	ChangeState(PlayerState::Damaged);
	//	return;
	//}

	
}

void Player::DamagedUpdate()
{

	if (CurDir_ == PlayerDir::Right)
	{
		MoveDir.x = -1.2f;
	}
	else if (CurDir_ == PlayerDir::Left)
	{
		MoveDir.x = 1.2f;
	}
	
	int LeftCheck = MapColImage_->GetImagePixel(GetPosition() + float4{ -20.f, 0.f });
	int RightCheck = MapColImage_->GetImagePixel(GetPosition() + float4{ 20.f, 0.f });

	float4 Pos = MoveDir;

	if (RGB(0, 0, 0) == LeftCheck)
	{
		MoveDir.x = 1.f;
	}
	else if (RGB(0, 0, 0) == RightCheck)
	{
		MoveDir.x = -1.f;
	}

	SetMove(MoveDir);

	if (PlayerAnimationRender->IsEndAnimation())
	{
		ChangeState(PlayerState::Idle);
		return;
	}
}

void Player::FullToMetalUpdate()
{
}

void Player::MetalTransfromUpdate()
{
}




//////////////////////////////////////// State

void Player::IdleStart()
{
	MoveDir = float4::ZERO;;

	Speed_ = 350.f;
	
	AnimationName_ = "Idle_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void Player::WalkStart()
{
	Speed_ = 350.f;

	AnimationName_ = "Walk_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void Player::RunStart()
{
	Speed_ = 500.f;

	AnimationName_ = "Run_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void Player::RunToStopStart()
{
	Speed_ = 350.f;
	StopTime_ = 0.3f;

	AnimationName_ = "RunToStop_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void Player::DownStart()
{
	DownTime_ = 0.5f;
	AnimationName_ = "Down_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);

}

void Player::SlideStart()
{
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

void Player::JumpStart()
{
	//FallTime_ = 0.8f;
	JumpPower_ = 1000.f;
	Gravity_ = 1800.f;

	// �� ���� 100�� ������ ���� ���� 
	MoveDir = float4::UP * JumpPower_;
	
	AnimationName_ = "Jump_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void Player::FloatStart()
{
	//FallTime_ = 0.8f;
	Speed_ = 3.f;
	Gravity_ = 300.f;

	PlayerAnimationRender->PauseOff();

	AnimationName_ = "Float_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
	
}

void Player::FallStart()
{
	MoveDir = float4::ZERO;;

	AnimationName_ = "Fall_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void Player::FallToBounceStart()
{
	Gravity_ = 0.0f;

	MoveDir = float4::ZERO;
	
	AnimationName_ = "FallToBounce_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void Player::BounceToIdleStart()
{
	MoveDir = float4::ZERO;

	AnimationName_ = "BounceToIdle_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void Player::InhaleStart()
{
	InhaleTime_ = 2.f;

	AnimationName_ = "Inhale_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void Player::FullStart()
{
	AnimationName_ = "Full_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void Player::FullWalkStart()
{
	AnimationName_ = "FullWalk_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void Player::FullJumpStart()
{
	AnimationName_ = "FullJump_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void Player::ExhaleStart()
{
	MoveDir = float4::ZERO;

	AnimationName_ = "Exhale_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void Player::SwallowStart()
{
	AnimationName_ = "Swallow_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void Player::ExhaustedStart()
{
	AnimationName_ = "Exhausted_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void Player::AttackStartStart()
{
	AnimationName_ = "AttackStart_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void Player::AttackStart()
{
	AnimationName_ = "Attack_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void Player::AttackEndStart()
{
	AnimationName_ = "AttackEnd_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void Player::DamagedStartStart()
{
	MoveDir = float4::ZERO;

	AnimationName_ = "DamagedStart_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void Player::DamagedStart()
{
	MoveDir = float4::ZERO;

	AnimationName_ = "Damaged_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void Player::FullToMetalStart()
{
}

void Player::MetalTrasformStart()
{
}



