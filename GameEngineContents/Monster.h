#pragma once
#include <GameEngine/GameEngineActor.h>

class Monster : public GameEngineActor
{
public:
	// constrcuter destructer
	Monster();
	~Monster();

	// delete Function
	Monster(const Monster& _Other) = delete;
	Monster(Monster&& _Other) noexcept = delete;
	Monster& operator=(const Monster& _Other) = delete;
	Monster& operator=(Monster&& _Other) noexcept = delete;

protected:

private:
	float Speed_;

	void Start() override;
	void Update() override;
	void Render() override;

};
