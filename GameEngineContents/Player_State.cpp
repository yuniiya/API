#include "Player.h"
#include <GameEngine/GameEngine.h>
#include <GameEngineBase/GameEngineWindow.h>
#include <GameEngine/GameEngineImageManager.h>
#include <GameEngineBase/GameEngineInput.h>
#include <GameEngineBase/GameEngineTime.h>
#include <GameEngine/GameEngineRenderer.h>

#include <GameEngine/GameEngineLevel.h> // ������ ���ؼ�
#include "Bullet.h" // �Ѿ��� ����� �ʹ�.

void Player::IdleUpdate()
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
}

void Player::WalkUpdate()
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

	
	// �޸���


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

	StagePixelCheck(Speed_);
}


void Player::RunUpdate()
{
	//DirAnimationCheck();

	if (false == IsMoveKey())
	{
		ChangeState(PlayerState::RunToStop);
		return;
	}

	StagePixelCheck(500.f);
	



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

	//	SetMove(MoveDir * GameEngineTime::GetDeltaTime() * Speed_);
	//	return;
	//}
}

void Player::RunToStopUpdate()
{
	MoveDir += -(MoveDir * 4.5f) * GameEngineTime::GetDeltaTime();
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
}

void Player::DownUpdate()
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

void Player::SlideUpdate()
{

	// ����
	MoveDir += -(MoveDir * 3.f) * GameEngineTime::GetDeltaTime();
	//SetMove(MoveDir);


	float4 CheckPos = GetPosition() + MoveDir * GameEngineTime::GetDeltaTime() * Speed_;
	int Color = MapColImage_->GetImagePixel(CheckPos);

	if (RGB(0, 0, 0) != Color)
	{
		SetMove(MoveDir * GameEngineTime::GetDeltaTime() * Speed_);
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
}

void Player::JumpUpdate()
{
	//if (PlayerAnimationRender->IsEndAnimation())
	//{
	//	if (false == IsMoveKey())
	//	{
	//		ChangeState(PlayerState::Idle);
	//		return;
	//	}
	//}

	SetMove(MoveDir * GameEngineTime::GetDeltaTime());


	MoveDir += float4::DOWN * GameEngineTime::GetDeltaTime() * 800.f;

	// �߷�
	

	//float4 CheckPos = GetPosition() + (MoveDir * GameEngineTime::GetDeltaTime() * 800.f);

	int Color = MapColImage_->GetImagePixel(GetPosition() + float4{0, 20});
	// ���� ��Ҵ�
	if (RGB(0, 0, 0) == Color)
	{
		MoveDir = float4::ZERO;

		ChangeState(PlayerState::Idle);
		return;
	}

	//if (true == GameEngineInput::GetInst()->IsPress("JumpLeft"))
	//{
	//	SetMove(float4::UP * GameEngineTime::GetDeltaTime() * Speed_);
	//}
	//if (true == GameEngineInput::GetInst()->IsPress("JumpRight"))
	//{
	//	SetMove(float4::UP * GameEngineTime::GetDeltaTime() * Speed_);
	//}

	//// �ƹ��� Ű�� ������ ������ ���� �����Ѵ�
	//if (false == IsMoveKey())
	//{
	//	MoveDir += -MoveDir * GameEngineTime::GetDeltaTime();

		//if (0.005f >= MoveDir.Len2D())
		//{
		//	MoveDir = float4::ZERO;
		//	return;
		//}

	//	SetMove(MoveDir * GameEngineTime::GetDeltaTime() * Speed_);
	//	return;
	//}

	//if (true == GameEngineInput::GetInst()->IsDown("Inhale"))
	//{
	//	ChangeState(PlayerState::Inhale);
	//	return;
	//}

}

void Player::FloatUpdate()
{

}

void Player::FallUpdate()
{
}

void Player::InhaleUpdate()
{
	if (true == IsMoveKey())
	{
		ChangeState(PlayerState::Walk);
		return;
	}

	if (true == IsJumpKey())
	{
		ChangeState(PlayerState::Jump);
		return;
	}
}

void Player::FullUpdate()
{
}

void Player::ExhaleUpdate()
{
}

void Player::SwallowUpdate()
{
}

void Player::AttackUpdate()
{

}

void Player::DamagedUpdate()
{
}




//////////////////////////////////////// State

void Player::IdleStart()
{
	// �ִϸ��̼��� �ٲ��.

	AnimationName_ = "Idle_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void Player::WalkStart()
{
	AnimationName_ = "Walk_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void Player::RunStart()
{
	AnimationName_ = "Run_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void Player::RunToStopStart()
{
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
	// �� ���� 100�� ������ ���� ���� 
	MoveDir = float4::UP * JumpPower_;

	AnimationName_ = "Jump_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void Player::FloatStart()
{
	AnimationName_ = "Float_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void Player::FallStart()
{
	AnimationName_ = "Fall_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void Player::InhaleStart()
{
	AnimationName_ = "Inhale_";
	//PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);

	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_ + "_Loop");
}

void Player::FullStart()
{
}

void Player::ExhaleStart()
{
}

void Player::SwallowStart()
{
}

void Player::AttackStart()
{

}

void Player::DamagedStart()
{
}



