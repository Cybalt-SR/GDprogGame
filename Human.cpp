
#include "Human.h"
#include "Vector2.h"
#include "Player.h"

Human::Human(Vector2 pos){

	this->position = pos;
	this->timeAvoided = 0;
	this->timeCanAvoid = 2;
	this->timeRested = 1;
	this->timeShouldRest = 1;
	this->leader = NULL;
}

Object * Human::getLeader() {

	return this->leader;
}

void Human::setLeader(Object * newLead) {

	this->leader = newLead;
}

void Human::Update(std::vector<Object*> nearby, float deltaTime) {

	Vector2 *moveVec = new Vector2();

	for (int i_n = 0; i_n < nearby.size(); i_n++)
	{
		auto i_cur = nearby[i_n];
		auto offset = i_cur->GetPos();
		offset -= this->position;
		float offsetDist = offset.SqrMagnitude();
		offset.Normalize();
		offset *= fmax(10 - offsetDist, 0.0f);

		*moveVec -= offset;
	}

	(*moveVec).Normalize();

	bool shouldAvoid = (*moveVec).SqrMagnitude() > 0 && timeAvoided < timeCanAvoid && timeRested >= timeShouldRest;
	bool led = leader != NULL;
	float speed = 4;

	if (led) {
		auto offset = leader->GetPos();
		offset -= this->position;
		offset *= 0.6f;

		*moveVec += offset;

		speed = 1;
	}

	if (shouldAvoid || led) {
		*moveVec *= deltaTime * speed;

		this->position += *moveVec;
		timeAvoided += deltaTime;
		timeRested = 0.0f;
	}
	else {
		timeAvoided = 0.0f;
		timeRested += deltaTime;
	}

	delete moveVec;
}

bool Human::IsDone() {

	return this->leader != NULL;
}

