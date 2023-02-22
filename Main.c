#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "h/Util.h"
#include "h/Entity.h"

void DisplayVersus(Entity *left, Entity *right)
{
    Display2ColTitle("STATS", "You", "Enemy");
    Display2ColValues("health", left->hp, right->hp);
    Display2ColValues("def   ", left->def, right->def);
    Display2ColValues("magic ", left->magic, right->magic);
}
void Combat(EntityAction Player, EntityAction Enemy){
    
}

int main()
{
    // Phase 1
    Entity Player = Entity.CreateEntity(1);
    // Phase 2
    Entity Enemy = Entity.CreateEntity(0);
    // Phase 3
    while (Player.hp > 0 || Enemy.hp > 0)
    {
        PrintDivider();
        DisplayVersus(&Player, &Enemy);
        Combat(Entity.GetAction(&Player, 0), Entity.GetAction(&Enemy, 1));
    }
}