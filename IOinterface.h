#pragma once
#include "Scene.h"
#include "Vector2.h"

class IOinterface {
  public:
    virtual void DisplayStart() = 0;

    virtual void DisplayLargeLevel(Scene , Vector2 ) = 0;

    virtual void DisplayLevelEnd() = 0;

    virtual void DisplayScene(Scene , Vector2 ) = 0;

    virtual Vector2 * GetMovementInput() = 0;

};
