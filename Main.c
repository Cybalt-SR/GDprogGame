/*
NOTE: When compiling, make sure to include ALL THREE *.c files in the compilation inputs.
    This is so that the linker will be able to link prototypes from the *.h files to their respective definitions in the *.c files.

FILES NEEDED TO INCLUDE:
    Main.c
    Entity.c
    u_list.c
*/

#include "h/u_stdio.h"

/*
Scope: Runtime Function
Use: Combines the functionality of Entity.h and u_stdio.h when it comes to displaying two-column values.
*/
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

/*
Scope: Runtime Function
Use: Event ordering. Which mostly uses a cointoss to determine order, alongside specific conditions.
*/
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

/*
Scope: Runtime Function
Use: Initializes or Reinitializes the game to its default values.
*/
static void Init(Entity **Player, Entity **Enemy, int *EnemiesDefeated)
{
    if (*Player != NULL) //To prevent memory leaks
    {
        free(*Player);
    }
    if (*Enemy != NULL) //To prevent memory leaks
    {
        free(*Enemy);
    }

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
    Entity *Player = NULL;
    Entity *Enemy = NULL;

    //Phase 1 and 2
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
        else //Someone has died
        {
            if (Player->hp > 0) // Win
            {
                Print("You killed an Enemy! \n", Colors.Green);

                if (AskInt("Do you wish to continue? [1/0] : ") == 1)
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
                    Print("Player health regained by %i.\n", Colors.Green, hpRegen);
                    if (defIncrease > 0)
                        Print("Player def increased by %i.\n", Colors.Green, defIncrease);
                    if (magIncrease > 0)
                        Print("Player magic regained by %i.\n", Colors.Green, magIncrease);
                }
            }
            else //Lose
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