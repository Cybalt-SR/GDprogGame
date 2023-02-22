#define ACTIONCOUNT 5

typedef struct Entity
{
    int hp, def, magic;
} Entity;

typedef struct EntityAction{
    void (*Action)(Entity *this, Entity *target);
    char *name;
} EntityAction;

extern const union EntityActionsType
{
    struct ActionsGroup{
        EntityAction Punch, Kick, Throw, MagicAtk, Block;
    }Specific;
    EntityAction All[ACTIONCOUNT];
}EntityActions;

extern const struct EntityUtilType
{
    void  (*AskAllocPoints)(int min, int max, int *remainingPoints, int *stat_field, char stat_label[]);
    Entity (*CreateEntity)(int byPlayer);
    EntityAction (*GetAction)(Entity *actioner, int randomAuto);
}EntityUtil;