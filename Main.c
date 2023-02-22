#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "h/Util.h"
#include "h/Entity.h"

void DisplayVersus(struct Entity *left, struct Entity *right)
{
    Display2ColTitle("STATS", "You", "Enemy");
    Display2ColValues("health", left->hp, right->hp);
    Display2ColValues("def   ", left->def, right->def);
    Display2ColValues("magic ", left->magic, right->magic);
}
void Combat(void (*Player)(struct Entity *this, struct Entity *target), void (*Enemy)(struct Entity *this, struct Entity *target)){
    
}

int main()
{
    // Phase 1
    struct Entity Player = Entity.CreateEntity(1);
    struct Entity Enemy = Entity.CreateEntity(0);

    while (Player.hp > 0 || Enemy.hp > 0)
    {
        PrintDivider();
        DisplayVersus(&Player, &Enemy);
        Entity.DisplayActions(&Player);
        int ActionId = AskInt("Action to do : ");
    }
}