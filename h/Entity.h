#define ACTIONCOUNT 5
#define MAGICACTIONID 3

struct Entity
{
    int hp, def, magic;
};
extern const struct EntityClass
{
    struct Entity (*new)(int _hp, int _def, int _magic);
    void  (*AskAllocPoints)(int min, int max, int *remainingPoints, int *stat_field, char stat_label[]);
    struct Entity (*CreateEntity)(int byPlayer);
    void (*AllActions[ACTIONCOUNT])(struct Entity *this, struct Entity *target);
    char *AllActionNames[ACTIONCOUNT];
    void (*DisplayActions)(void);
} Entity;