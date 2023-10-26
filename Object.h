#pragma once
#include <vector>
#include "Vector2.h"


class Object {
protected:
	Vector2 position;
public:
	virtual void Update(std::vector<Object*>, float) = 0;
	virtual bool IsDone() = 0;
	Vector2 GetPos() { return position; };
};
