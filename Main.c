#include "h/u_stdio.h"

void Display2ColEntityInfo(Entity *left, Entity *right)
{
    int leftActualDef = left->def;
    int rightActualDef = right->def;
    List.GetTotal(left->DefModifiers, &leftActualDef);
    List.GetTotal(right->DefModifiers, &rightActualDef);

    Display2ColTitle("STATS", left->name, right->name);
    Display2ColValues("health", left->hp, right->hp);
    Display2ColValues("def   ", leftActualDef, rightActualDef);
    Display2ColValues("magic ", left->magic, right->magic);
}
void Combat(EntityEvent PlayerEvent, EntityEvent EnemyEvent)
{
    // Setting default execution order
    EntityEvent *firstEvent = &PlayerEvent;
    EntityEvent *secondEvent = &EnemyEvent;

    // If any blocked
    if (EnemyEvent.action->Action == EntityActions.S.Block.Action)
    {
        firstEvent = &EnemyEvent;
        secondEvent = &PlayerEvent;
    } // No need for reverse statement because even if the Player blocks too, the default order is already player-first
    else if (PlayerEvent.action->requiresCoinToss || EnemyEvent.action->requiresCoinToss)
    {
        int coinToss = CoinToss();

        if (coinToss == 0)
        {
            firstEvent = &EnemyEvent;
            secondEvent = &PlayerEvent;
        } // No need for an else statement because the default order is already player-first.
    }

    firstEvent->action->Action(firstEvent->doer, secondEvent->doer);
    secondEvent->action->Action(secondEvent->doer, firstEvent->doer);
}

static void Init(Entity **Player, Entity **Enemy, int *EnemiesDefeated)
{
    *EnemiesDefeated = 0;
    // Phase 1
    *Player = EntityConstructor.CreatePlayer("You", "Your");
    // Phase 2
    *Enemy = EntityConstructor.CreateBot("Enemy", "Their", *EnemiesDefeated);
}

int main()
{
    int userQuitted = 0;

    int EnemiesDefeated = 0;
    Entity *Player;
    Entity *Enemy;

    Init(&Player, &Enemy, &EnemiesDefeated);

    // Phase 3
    while (userQuitted != 1)
    {
        if (Player->hp > 0 && Enemy->hp > 0)
        {
            PrintDivider();
            Display2ColEntityInfo(Player, Enemy);
            PrintDivider();
            Combat(Player->GetActionEvent(Player), Enemy->GetActionEvent(Enemy));

            List.UpdateTick(Player->DefModifiers);
            List.UpdateTick(Enemy->DefModifiers);
        }
        else
        {
            if (Player->hp > 0 && AskInt("Do you wish to continue? [1/0] : ") == 1) // Win
            {
                // Enemy reinit
                EnemiesDefeated++;
                free(Enemy);
                Enemy = EntityConstructor.CreateBot("Enemy", "Their", EnemiesDefeated);

                // Player reinit
                int hpRegen = RandomRange(1, 30);
                int defIncrease = Player->def < 9 ? RandomRange(0, 1) : 0;
                int magIncrease = RandomRange(0, 3);

                Player->hp += hpRegen;
                Player->def += defIncrease;
                Player->magic += magIncrease;

                // Display
                PrintDivider();
                Print("You killed an Enemy! \n", Colors.Green);
                Print("Player health regained by %i.\n", Colors.Green, hpRegen);
                if (defIncrease > 0)
                    Print("Player def increased by %i.\n", Colors.Green, defIncrease);
                if (magIncrease > 0)
                    Print("Player magic regained by %i.\n", Colors.Green, magIncrease);
            }
            else
            {
                Print("You are dead.\n", Colors.Red);

                if (AskInt("Do you wish to restart? [1/0]: ") == 1)
                {
                    Init(&Player, &Enemy, &EnemiesDefeated);
                }
                else
                {
                    userQuitted = 1;
                }
            }
        }
    }
}