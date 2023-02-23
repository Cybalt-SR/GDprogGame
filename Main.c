#include "h/Util.h"
#include "h/EntityList.h"
#include "h/Entity.h"

void Display2ColEntityInfo(Entity *left, Entity *right)
{
    int leftActualDef = left->def;
    int rightActualDef = right->def;
    EntityList.GetTotal(left->DefModifiers, &leftActualDef);
    EntityList.GetTotal(right->DefModifiers, &rightActualDef);

    Display2ColTitle("STATS", left->name, right->name);
    Display2ColValues("health", left->hp, right->hp);
    Display2ColValues("def   ", leftActualDef, rightActualDef);
    Display2ColValues("magic ", left->magic, right->magic);
}
void Combat(EntityEvent PlayerEvent, EntityEvent EnemyEvent)
{
    //Setting default execution order
    EntityEvent *firstEvent = &PlayerEvent;
    EntityEvent *secondEvent = &EnemyEvent;

    // If any blocked
    if (EnemyEvent.action->Action == EntityActions.S.Block.Action)
    {
        firstEvent = &EnemyEvent;
        secondEvent = &PlayerEvent;
    } // No need for reverse statement because even if the Player blocks too, the default order is already player-first
    else if (PlayerEvent.action->requiresCoinToss || EnemyEvent.action->requiresCoinToss)// If Actions requires cointoss
    {
        int coinToss = CoinToss();

        if (coinToss == 0)
        {
            firstEvent = &EnemyEvent;
            secondEvent = &PlayerEvent;
        } // No need for an else statement because the default order is player-first.
    }

    firstEvent->action->Action(firstEvent->doer, secondEvent->doer);
    secondEvent->action->Action(secondEvent->doer, firstEvent->doer);
}

int main()
{
    // Phase 1
    Entity Player = EntityConstructor.Create("You", 1);
    // Phase 2
    Entity Enemy = EntityConstructor.Create("Enemy", 0);
    // Phase 3
    while (Player.hp > 0 || Enemy.hp > 0)
    {
        EntityList.UpdateTick(Player.DefModifiers);
        EntityList.UpdateTick(Enemy.DefModifiers);

        PrintDivider();
        Display2ColEntityInfo(&Player, &Enemy);
        PrintDivider();
        Combat(Player.GetActionEvent(&Player), Enemy.GetActionEvent(&Enemy));
    }
}