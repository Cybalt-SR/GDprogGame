#pragma once
#include "Object.h"
#include "Vector2.h"

class Player : public Object {
private:
	float speed;
	Vector2 MoveDir;
public:
	Player(Vector2 pos);

	virtual void Update(std::vector<Object*> objects, float deltaTime) override;

	virtual bool IsDone() override;

	void SetMoveDir(Vector2 moveVec);
};
