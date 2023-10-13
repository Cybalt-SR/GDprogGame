
#include "Player.h"
#include "Vector2.h"
#include "Human.h"

Player::Player(Vector2 pos){

	this->position = pos;
	this->speed = 1.6f;
}

void Player::Update(std::vector<Object*> objects, float deltaTime) {

	Vector2 *deltaMove = new Vector2(MoveDir.x(), MoveDir.y());
	*deltaMove *= deltaTime * speed;

	this->position += *deltaMove;

	for (int i_o = 0; i_o < objects.size(); i_o++)
	{
		auto human = dynamic_cast<Human*>(objects[i_o]);

		if (human != NULL && Vector2::SqrDistance(human->GetPos(), this->position) <= 0.7f * 0.7f) {
			human->setLeader(this);
		}
	}

	delete deltaMove;
}

bool Player::IsDone() {

	return true;
}

void Player::SetMoveDir(Vector2 moveVec) {

	this->MoveDir = moveVec;
}

