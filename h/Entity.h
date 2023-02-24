#define ACTIONCOUNT 5

struct Entity
{
    char *name;
    char *pronoun;
    int hp, def, magic; //base values
    struct EntityEvent (*GetActionEvent)(struct Entity *actioner);
    EntityListElement DefModifiers;
};

struct EntityAction
{
    void (*Action)(struct Entity *this, struct Entity *target);
    char *name;
    int requiresCoinToss;
};

struct EntityEvent
{
    struct Entity *doer;
    struct EntityAction *action;
};

typedef struct Entity Entity;
typedef struct EntityAction EntityAction;
typedef struct EntityEvent EntityEvent;

extern const union EntityActions
{
    struct ActionsGroup
    {
        EntityAction Punch, Kick, Throw, MagicAtk, Block;
    } S;
    EntityAction All[ACTIONCOUNT];
} EntityActions;

extern const struct EntityConstructor
{
    Entity *(*Create)(char name[], char pronoun[], int byPlayer);
} EntityConstructor;