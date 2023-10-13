#pragma once
#include "IOinterface.h"

#include <iostream>
static class ConsoleIO :
    public IOinterface
{
public:
    virtual void DisplayStart() override;
    virtual void DisplayLargeLevel(Scene, Vector2) override;
    virtual void DisplayLevelEnd() override;
    virtual void DisplayScene(Scene, Vector2) override;
    virtual Vector2* GetMovementInput() override;
};

