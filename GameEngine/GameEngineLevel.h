#pragma once
#include <list>
#include <map>
#include <vector>
#include <GameEngineBase/GameEngineNameObject.h>
#include <GameEngineBase/GameEngineMath.h>

class GameEngineActor;
struct ChangeOrderItem
{
	GameEngineActor* TargetObject; // �̳༮��
	int ChangeOrder; // �̷��� �ٲ��.
};


class GameEngine;
class GameEngineActor;
class GameEngineCollision;
class GameEngineRenderer;
class GameEngineLevel : public GameEngineNameObject
{
	// ���������� ���� �˾Ƶ� ��� ����
	friend GameEngine;
	friend GameEngineActor;
	friend GameEngineCollision;
	friend GameEngineRenderer;

public:
	// constrcuter destructer
	GameEngineLevel();
	// 
	virtual ~GameEngineLevel();

	// delete Function
	GameEngineLevel(const GameEngineLevel& _Other) = delete;
	GameEngineLevel(GameEngineLevel&& _Other) noexcept = delete;
	GameEngineLevel& operator=(const GameEngineLevel& _Other) = delete;
	GameEngineLevel& operator=(GameEngineLevel&& _Other) noexcept = delete;

	template<typename ActorType>
	ActorType* CreateActor(int _Order = 0, const std::string& _Name = "")
	{
		// Actor�� �� ���� ������ �̸� �� �� �����ϱ� �����Ҵ�
		ActorType* NewActor = new ActorType();
		// Actor�� Start�ϱ� ���� Actor�� ��ĳ���� �ʿ�
		GameEngineActor* StartActor = NewActor;
		NewActor->SetOrder(_Order);
		NewActor->SetName(_Name);
		NewActor->SetLevel(this);

		StartActor->Start();

		// AllActor���� _Order�� find�� ������ ���� ���� insert
		// ���ο� Actor�� list�� �־��ش� 
		std::list<GameEngineActor*>& Group = AllActor_[_Order];
		Group.push_back(NewActor);

		return NewActor;
	}

	inline float4 GetCameraPos()
	{
		return CameraPos_;
	}

	inline void MoveCameraPos(const float4& _Value)
	{
		CameraPos_ += _Value;
	}

	inline void SetCameraPos(const float4& _Value)
	{
		CameraPos_ = _Value;
	}

	template<typename ConvertType>
	ConvertType* FindActor(const std::string& _Name)
	{
		return dynamic_cast<ConvertType*>(FindActor(_Name));
	}

	GameEngineActor* FindActor(const std::string& _Name);

	void RegistActor(const std::string& _Name, GameEngineActor* _Actor);


protected:
	// ���� �Լ�
	// ��������鼭 ���ҽ��� ���͸� ���鶧 ���
	virtual void Loading() = 0;
	// �� ������ ���� �����϶� �ؾ������� �����Ѵ�.
	virtual void Update() = 0;

	// �߻�ȭ���� �ʾұ� ������ ������ ��� ������ �ʿ� 
	// Start: ���� ����->���� ������ ������ �� ���� ������ �����ϴ� �Լ�
	// End : ���� ����->���� ������ ������ �� ���� ������ �����ϴ� �Լ�
	void ActorLevelChangeStart();
	virtual void LevelChangeStart() {}

	void ActorLevelChangeEnd();
	virtual void LevelChangeEnd() {}

private:
	// ��� Actor�� map���� ���� 
	std::map<int, std::list<GameEngineActor*>> AllActor_;

	std::map<std::string, GameEngineActor*> RegistActor_;

	std::vector<ChangeOrderItem> ChangeOrderList;

	float4 CameraPos_;

	void ActorUpdate();
	void ActorRender();
	void CollisionDebugRender();
	void ActorRelease();

private:
	std::map<int, std::list<GameEngineRenderer*>> AllRenderer_;

	void AddRenderer(GameEngineRenderer* _Renderer);

	void ChangeUpdateOrder(GameEngineActor* _Actor, int _Oreder);

	void ChangeRenderOrder(GameEngineRenderer* _Renderer, int _NewOrder);

private:
	// ������ ���Ͱ� ������ ���� ����� Level
	// ���⼭ �Ժη� GameEngineCollision*�� delete �ϴ� ���� ������ �ȵȴ�
	std::map<std::string, std::list<GameEngineCollision*>> AllCollision_;

	void AddCollision(const std::string& _GroupName, GameEngineCollision* _Collision);
};
