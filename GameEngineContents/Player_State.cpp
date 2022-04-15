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
	if (true == IsMoveKey())
	{
		ChangeState(PlayerState::Walk);
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

	// ���� 
	if (true == GameEngineInput::GetInst()->IsDown("JumpLeft") || true == GameEngineInput::GetInst()->IsDown("JumpRight"))
	{
		ChangeState(PlayerState::Jump);
		return;
	}

	DirAnimationCheck();

	MoveDir = float4::ZERO;


	if (true == GameEngineInput::GetInst()->IsPress("MoveLeft"))
	{
		MoveDir = float4::LEFT;
		//SetMove(float4::LEFT * GameEngineTime::GetDeltaTime() * Speed_);
	}

	if (true == GameEngineInput::GetInst()->IsPress("MoveRight"))
	{
		MoveDir = float4::RIGHT;
		//SetMove(float4::RIGHT * GameEngineTime::GetDeltaTime() * Speed_);

		// ����
		// MoveDir += float4::RIGHT * GameEngineTime::GetDeltaTime() * AccSpeed_;
	}

	if (true == GameEngineInput::GetInst()->IsPress("MoveUp"))
	{
		MoveDir = float4::UP;
		//SetMove(float4::UP * GameEngineTime::GetDeltaTime() * Speed_);
	}

	if (true == GameEngineInput::GetInst()->IsPress("MoveDown"))
	{
		MoveDir = float4::DOWN;
		//SetMove(float4::DOWN * GameEngineTime::GetDeltaTime() * Speed_);
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
		SetMove(MoveDir * GameEngineTime::GetDeltaTime() * Speed_);
	}
}


void Player::RunUpdate()
{
	// �ӷ� ����
	if (0.3f <= MoveDir.Len2D())
	{
		MoveDir.Range2D(0.3f);
	}

	// �ƹ��� Ű�� ������ ������ ���� �����Ѵ�
	if (false == IsMoveKey())
	{
		MoveDir += -MoveDir * GameEngineTime::GetDeltaTime();

		if (0.005f >= MoveDir.Len2D())
		{
			MoveDir = float4::ZERO;
			return;
		}

		SetMove(MoveDir * GameEngineTime::GetDeltaTime() * Speed_);
		return;
	}
}

void Player::RunToStopUpdate()
{
}

void Player::DownUpdate()
{
}

void Player::SlideUpdate()
{
}

void Player::JumpUpdate()
{
	SetMove(MoveDir * GameEngineTime::GetDeltaTime());

	// �߷�
	MoveDir += float4::DOWN * GameEngineTime::GetDeltaTime() * 1000.0f;


	if (true == GameEngineInput::GetInst()->IsPress("JumpLeft"))
	{
		SetMove(float4::UP * GameEngineTime::GetDeltaTime() * Speed_);
	}
	if (true == GameEngineInput::GetInst()->IsPress("JumpRight"))
	{
		SetMove(float4::UP * GameEngineTime::GetDeltaTime() * Speed_);
	}

	// �ƹ��� Ű�� ������ ������ ���� �����Ѵ�
	if (false == IsMoveKey())
	{
		MoveDir += -MoveDir * GameEngineTime::GetDeltaTime();

		if (0.005f >= MoveDir.Len2D())
		{
			MoveDir = float4::ZERO;
			return;
		}

		SetMove(MoveDir * GameEngineTime::GetDeltaTime() * Speed_);
		return;
	}

}

void Player::FloatUpdate()
{
}

void Player::FallUpdate()
{
}

void Player::InhaleUpdate()
{
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
	//PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void Player::WalkStart()
{
	AnimationName_ = "Walk_";
	//PlayerAnimationRender->ChangeAnimation(AnimationName_ + ChangeDirText_);
}

void Player::RunStart()
{
}

void Player::RunToStopStart()
{
}

void Player::DownStart()
{
}

void Player::SlideStart()
{
}

void Player::JumpStart()
{
}

void Player::FloatStart()
{
}

void Player::FallStart()
{
}

void Player::InhaleStart()
{
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



