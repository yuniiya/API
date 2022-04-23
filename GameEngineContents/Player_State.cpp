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


	// ���� Ű ������ 0.03�̻��̸� LongJump
	//if (GameEngineInput::GetInst()->GetTime("JumpRight") >= 0.03f)
	//{
	//	LongJump_ = true;
	//}
	//else if (GameEngineInput::GetInst()->GetTime("JumpRight") < 0.005f)
	//{
	//	LongJump_ = false;
	//}
	//	

	// ���� 
	if (true == IsJumpKey())
	{
		//if (true == LongJump_)
		//{
		//	JumpPower_ = 1000.f;
		//	Gravity_ = 1800.f;
		//}
		//else if (false == LongJump_)
		//{
		//	JumpPower_ = 500.f;
		//	Gravity_ = 900.f;
		//}

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

	Move();
	StagePixelCheck(Speed_);

	// ������, �������� 
	float4 RightDownkPos = GetPosition() + float4{ 0.f,20.f };
	float4 LeftUpPos = GetPosition() + float4{ -20.f,0.f };

	int DownColor = MapColImage_->GetImagePixel(RightDownkPos);
	int UpColor = MapColImage_->GetImagePixel(LeftUpPos);


	float4 XMove = { MoveDir.x, 0.0f };
	float4 YMove = { 0.0f, MoveDir.y - 1.f };

	if (RGB(0, 0, 0) != DownColor)
	{
		SetMove(float4::DOWN);
	}
	else if (RGB(0, 0, 0) != UpColor)
	{
		SetMove(YMove);
	}
}


void Player::RunUpdate()
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



	// ������, �������� 
	float4 RightDownkPos = GetPosition() + float4{ 0.f,20.f };
	float4 LeftUpPos = GetPosition() + float4{ -20.f,0.f };

	int DownColor = MapColImage_->GetImagePixel(RightDownkPos);
	int UpColor = MapColImage_->GetImagePixel(LeftUpPos);


	float4 XMove = { MoveDir.x, 0.0f };
	float4 YMove = { 0.0f, MoveDir.y - 1.f };

	if (RGB(0, 0, 0) != DownColor)
	{
		SetMove(float4::DOWN);
	}
	else if (RGB(0, 0, 0) != UpColor)
	{
		SetMove(YMove);
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
	float4 RightDownkPos = GetPosition() + float4{ 0.f,20.f };
	float4 LeftUpPos = GetPosition() + float4{ -20.f,0.f };

	int DownColor = MapColImage_->GetImagePixel(RightDownkPos);
	int UpColor = MapColImage_->GetImagePixel(LeftUpPos);

	float4 XMove = { MoveDir.x, 0.0f };
	float4 YMove = { 0.0f, MoveDir.y - 1.f };

	if (RGB(0, 0, 0) != DownColor)
	{ 
		SetMove(float4::DOWN);
	}
	else if (RGB(0, 0, 0) != UpColor)
	{
		SetMove(YMove);
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
	float4 RightDownkPos = GetPosition() + float4{ 0.f,20.f };
	float4 LeftUpPos = GetPosition() + float4{ -20.f,0.f };

	int RightDownColor = MapColImage_->GetImagePixel(RightDownkPos);
	int LeftUpColor = MapColImage_->GetImagePixel(LeftUpPos);


	float4 XMove = { MoveDir.x, 0.0f };
	float4 YMove = { 0.0f, MoveDir.y - 1.f };

	if (RGB(0, 0, 0) != RightDownColor)
	{
		SetMove(float4::DOWN);
	}
	else if (RGB(0, 0, 0) != LeftUpColor)
	{
		SetMove(YMove);
	}
}

void Player::JumpUpdate()
{
	// ���� �̵�
	SetMove(MoveDir * GameEngineTime::GetDeltaTime());

	float4 XPos = { MoveDir.x, 0.0f };
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
		MoveDir += float4::LEFT;
	}
	else if (true == GameEngineInput::GetInst()->IsPress("MoveRight"))
	{
		MoveDir += float4::RIGHT;
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
	float4 CheckPos = GetPosition() + float4{0, 20.f};
	int Color = MapColImage_->GetImagePixel(CheckPos);


	float4 CheckPos2 = GetPosition() + float4{ 0, 180.f };
	int Color2 = MapColImage_->GetImagePixel(CheckPos2);

	if (RGB(0, 0, 0) == Color)
	{
		MoveDir = float4::ZERO;
		ChangeState(PlayerState::Idle);
		return;
	}
	else if (RGB(0, 0, 0) != Color2)
	{
		FallTime_ -= GameEngineTime::GetDeltaTime();
		if (FallTime_ <= 0.f)
		{
			ChangeState(PlayerState::Fall);
			return;
		}
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
	// ���� ����� ��������
	if (GameEngineInput::GetInst()->IsDown("Inhale"))
	{
		ChangeState(PlayerState::Exhale);
		return;
	}

	if (PlayerAnimationRender->IsEndAnimation())
	{
		PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_ + "_Loop");
	}

	// Float���¿��� �̵�
	MoveDir = float4::ZERO;

	if (true == GameEngineInput::GetInst()->IsPress("MoveLeft"))
	{
		MoveDir = float4{-0.8f, 0.f};
		PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_ + "_Loop");
	}
	else if (true == GameEngineInput::GetInst()->IsPress("MoveRight"))
	{
		MoveDir = float4{0.8f, 0.f};
		PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_ + "_Loop");
	}
	else if (true == GameEngineInput::GetInst()->IsPress("MoveUp"))
	{
		MoveDir = float4{ 0.f, -0.8f};
	}

	// �߷�
	if (false == IsJumpKey())
	{
		GravityOn();
		SetMove(MoveDir);
	}
	else if (true == IsJumpKey())
	{
		MoveDir.y = -0.5f;
		SetMove(MoveDir);
	}

	// �� �� + �� �ȼ� üũ
	MovePixelCheck(20.0f, 20.0f);

	// Float���·� �ٴڿ� ��Ҵ�
	float4 CheckPos = GetPosition() + float4{ 0, 20.f };

	int Color = MapColImage_->GetImagePixel(CheckPos);
	if (RGB(0, 0, 0) == Color)
	{
		MoveDir = float4::UP;
		SetMove(MoveDir);
	}


}

void Player::FallUpdate()
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


	// �Ʒ��� ��������
	SetMove(MoveDir * GameEngineTime::GetDeltaTime());


	float4 XPos = { MoveDir.x, 0.0f };
	float4 YPos = { 0.0f, MoveDir.y };

	// ���� ���� ���ϴ� �ִϸ��̼� ���
	if (1 == PlayerAnimationRender->CurrentAnimation()->WorldCurrentFrame())
	{
		if (YPos.y >= 150.f)
		{
			PlayerAnimationRender->PauseOn();
			YPos = { 0.0f, MoveDir.y };
		}
	}

	// ���� ��������� �Ӹ��� ���� ���ϴ� �ִϸ��̼� ���
	float4 StageCheckPos = GetPosition() + float4{ 0, 200.f };
	int StageColor = MapColImage_->GetImagePixel(StageCheckPos);
	if (RGB(0, 0, 0) == StageColor)
	{
		PlayerAnimationRender->PauseOff();

	}

	// ���� ��� �� ������ 4��° �ִϸ��̼ǿ��� Pause
	if (4 == PlayerAnimationRender->CurrentAnimation()->WorldCurrentFrame())
	{
		PlayerAnimationRender->PauseOn();
		StageCheckPos = {0.0f, 0.0f};
	}

	// ���� ������ Pause ���� �� �� �� ƨ���
	StageCheckPos = GetPosition() + float4{ 0, 20.f };
	StageColor = MapColImage_->GetImagePixel(StageCheckPos);
	if (RGB(0, 0, 0) == StageColor)
	{
		MoveDir = float4{ MoveDir.x, -1.f } * 400.f;
		SetMove(MoveDir * GameEngineTime::GetDeltaTime());
		Gravity_ = 1200.f;
		
	}

	GravityOn();

	// ƨ���
	if (Gravity_ >= 1200.f)
	{
		PlayerAnimationRender->PauseOff();
	}


	// ƨ�� �� �ٽ� ���� ���� => Idle
	StageCheckPos = GetPosition() + float4{ 0, 40.f };
	StageColor = MapColImage_->GetImagePixel(StageCheckPos);
	if (RGB(0, 0, 0) == StageColor
		&& PlayerAnimationRender->IsEndAnimation())
	{


		MoveDir = float4::ZERO;
		ChangeState(PlayerState::Idle);
		return;
	}

}

void Player::InhaleUpdate()
{
	InhaleTime_ -= GameEngineTime::GetDeltaTime();

	if (PlayerAnimationRender->IsEndAnimation())
	{
		PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_ + "_Loop");
	}
	
	// 2.5�� �� Exhausted
	if (InhaleTime_ <= 0)
	{
		ChangeState(PlayerState::Exhausted);
		return;
	}

	// Ű���� �� ���� �� -> Idle
	if(GameEngineInput::GetInst()->IsUp("Inhale"))
	{
		ChangeState(PlayerState::Idle);
		return;
	}

	// �ȱ�
	if (true == IsMoveKey())
	{
		ChangeState(PlayerState::Walk);
		return;
	}

	// ����
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
	if (PlayerAnimationRender->IsEndAnimation())
	{
		ChangeState(PlayerState::Fall);
		return;
	}
	
	
}

void Player::SwallowUpdate()
{
}

void Player::ExhaustedUpdate()
{
	ChangeState(PlayerState::Idle);
	return;

	//if (PlayerAnimationRender->IsEndAnimation())
	//{
	//	
	//}
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
	Speed_ = 350.f;

	/*JumpPower_ = 1000.f;
	Gravity_ = 1800.f;*/
	
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
	FallTime_ = 0.8f;
	JumpPower_ = 1000.f;
	Gravity_ = 1800.f;

	// �� ���� 100�� ������ ���� ���� 
	MoveDir = float4::UP * JumpPower_;
	
	AnimationName_ = "Jump_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void Player::FloatStart()
{
	FallTime_ = 0.8f;
	Speed_ = 5.f;
	Gravity_ = 100.f;

	AnimationName_ = "Float_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
	
}

void Player::FallStart()
{
	Gravity_ = 500.f;

	MoveDir = float4::DOWN * Gravity_;

	AnimationName_ = "Fall_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void Player::InhaleStart()
{
	InhaleTime_ = 2.5f;

	AnimationName_ = "Inhale_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void Player::FullStart()
{
}

void Player::ExhaleStart()
{
	AnimationName_ = "Exhale_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void Player::SwallowStart()
{
}

void Player::ExhaustedStart()
{
	AnimationName_ = "Exhausted_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void Player::AttackStart()
{

}

void Player::DamagedStart()
{
}



