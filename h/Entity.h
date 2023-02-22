#define ACTIONCOUNT 5

typedef struct EntityClass Entity;
typedef void (*EntityAction)(Entity *this, Entity *target);

struct EntityClass
{
    int hp, def, magic;
};
extern const struct EntityNamespace
{
    Entity (*new)(int _hp, int _def, int _magic);
    void  (*AskAllocPoints)(int min, int max, int *remainingPoints, int *stat_field, char stat_label[]);
    Entity (*CreateEntity)(int byPlayer);
    EntityAction AllActions[ACTIONCOUNT];
    char *AllActionNames[ACTIONCOUNT];
    EntityAction (*GetAction)(Entity *actioner, int randomAuto);
} EntityNamespace;