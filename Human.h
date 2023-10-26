#pragma once


#include "Object.h"
#include <vector>

class Vector2;
class Player;

class Human : public Object {
private:
	float timeAvoided;
	float timeCanAvoid;
	float timeRested;
	float timeShouldRest;
	Object* leader;
public:
	Human(Vector2 pos);
	Object* getLeader();
	void setLeader(Object* newLead);
	virtual void Update(std::vector<Object*> nearby, float deltaTime) override;
	virtual bool IsDone() override;
};
