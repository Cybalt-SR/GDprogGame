#define ACTIONCOUNT 5

struct Entity
{
    int hp, def, magic; //base values
    struct EntityEvent (*GetActionEvent)(struct Entity *actioner);
    EntityListElement DefModifiers;
};

struct EntityAction
{
    void (*Action)(struct Entity *this, struct Entity *target);
    char *name;
};

struct EntityEvent
{
    struct Entity *enacter;
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
    } Specific;
    EntityAction All[ACTIONCOUNT];
} EntityActions;

extern const struct EntityConstructor
{
    Entity (*Create)(int byPlayer);
} EntityConstructor;