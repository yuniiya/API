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
	if (true == GameEngineInput::GetInst()->IsDown("MoveLeft") || true == GameEngineInput::GetInst()->IsDown("MoveRight"))
	{
		ChangeState(PlayerState::Walk);
		return;
	}
	
	if (true == GameEngineInput::GetInst()->IsPress("Down"))
	{
		ChangeState(PlayerState::Down);
		return;

		if (true == IsMoveKey())
		{
			ChangeState(PlayerState::Slide);
			return;
		}
	}

	if (true == GameEngineInput::GetInst()->IsPress("Inhale"))
	{
		ChangeState(PlayerState::Inhale);
		return;
	}

	// ���� 
	if (true == GameEngineInput::GetInst()->IsDown("JumpLeft") || true == GameEngineInput::GetInst()->IsDown("JumpRight"))
	{
		ChangeState(PlayerState::Jump);
		return;
	}
}

void Player::WalkUpdate()
{
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
	if (true == GameEngineInput::GetInst()->IsPress("Run") && true == IsMoveKey())
	{
		ChangeState(PlayerState::Run);
		return;
	}

	// ���� 
	if (true == GameEngineInput::GetInst()->IsDown("JumpLeft") || true == GameEngineInput::GetInst()->IsDown("JumpRight"))
	{
		ChangeState(PlayerState::Jump);
		return;
	}

	if (true == GameEngineInput::GetInst()->IsPress("Inhale"))
	{
		ChangeState(PlayerState::Inhale);
		return;
	}

	StagePixelCheck();
}


void Player::RunUpdate()
{
	//DirAnimationCheck();


	MoveDir = float4::ZERO;

	if (true == GameEngineInput::GetInst()->IsPress("MoveLeft"))
	{
		MoveDir = float4::LEFT;
	}

	if (true == GameEngineInput::GetInst()->IsPress("MoveRight"))
	{
		MoveDir = float4::RIGHT;
	}

	// �̷� ��ġ -> �̵��Ѵٸ� �� ��ġ�� ���������� ��
	float4 NextPos = GetPosition() + (MoveDir * GameEngineTime::GetDeltaTime() * Speed_);
	// �̷� ��ġ���� �߽����� �߳����� ���� ��ġ
	float4 CheckPos = NextPos + float4(0.0f, 25.0f);

	// �÷��̾� ��ġ + N = �÷��̾� �� �� (N�� ������ ���� �־ �� ������ ��ġ ������ �� �ִ�)
	// �浹���� ���� �����ͼ� 
	int Color = MapColImage_->GetImagePixel(CheckPos);
	// �ʷϻ���(����) �ƴ϶�� && ī�޶� �ٱ� ������ �ƴ϶�� -> �̵� 
	if (RGB(0, 255, 0) != Color && RGB(0, 0, 0) != Color)
	{
		SetMove(MoveDir * GameEngineTime::GetDeltaTime() * 550.f);
	}


	if (true == IsMoveKey())
	{
		ChangeState(PlayerState::Walk);
		return;
	}

	if (false == IsMoveKey())
	{
		ChangeState(PlayerState::Idle);
		return;
	}

	//if (true == GameEngineInput::GetInst()->IsDown("MoveRight"))
	//{
	//	ChangeState(PlayerState::Run);
	//	return;
	//}

	if (true == GameEngineInput::GetInst()->IsDown("JumpLeft") || true == GameEngineInput::GetInst()->IsDown("JumpRight"))
	{
		ChangeState(PlayerState::Jump);
		return;
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

	//	SetMove(MoveDir * GameEngineTime::GetDeltaTime() * Speed_);
	//	return;
	//}
}

void Player::RunToStopUpdate()
{
}

void Player::DownUpdate()
{
	if (PlayerAnimationRender->IsEndAnimation())
	{
		ChangeState(PlayerState::Idle);
	}

	if (false == IsMoveKey())
	{
		ChangeState(PlayerState::Idle);
		return;
	}

	if (true == IsMoveKey())
	{
		ChangeState(PlayerState::Walk);
		return;
	}
}

void Player::SlideUpdate()
{
	if (false == IsMoveKey())
	{
		ChangeState(PlayerState::Idle);
		return;
	}

	if (true == IsMoveKey())
	{
		ChangeState(PlayerState::Walk);
		return;
	}

}

void Player::JumpUpdate()
{
	if (true == IsMoveKey())
	{
		ChangeState(PlayerState::Walk);
		return;
	}

	//SetMove(MoveDir * GameEngineTime::GetDeltaTime());

	//// �߷�
	//MoveDir += float4::DOWN * GameEngineTime::GetDeltaTime() * 1000.0f;


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

	//	if (0.005f >= MoveDir.Len2D())
	//	{
	//		MoveDir = float4::ZERO;
	//		return;
	//	}

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
	AnimationName_ = "RunToStop_";
}

void Player::DownStart()
{
	AnimationName_ = "Down_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);

}

void Player::SlideStart()
{
	AnimationName_ = "Slide_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);

	if (ChangeDirText_ == "Right")
		MoveDir += float4::RIGHT;
	else
		MoveDir += float4::LEFT;
}

void Player::JumpStart()
{
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
}

void Player::InhaleStart()
{
	AnimationName_ = "Inhale_";
	PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
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



