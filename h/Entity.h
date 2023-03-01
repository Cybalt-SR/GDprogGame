#define ACTIONCOUNT 5

/*
Used to group variables relating to a single entity and specific to that Entity.
Made so that dealing with combat is more modular and scalable... a good practice.
*/
struct Entity
{
    char *name;
    char *pronoun;
    int hp, def, magic; //base values
    struct EntityEvent (*GetActionEvent)(struct Entity *actioner);
    ListElement DefModifiers;
};


//Used to identify a type of targeted action that an entity can do.
struct EntityAction
{
    void (*Action)(struct Entity *this, struct Entity *target);
    char *name;
    int requiresCoinToss;
};


//Used group an action alongside its actioner/doer.
struct EntityEvent
{
    struct Entity *doer;
    struct EntityAction *action;
};

/*
typedefs used so that there will be minimal confusion between struct keywords being used in the code. 
*/

//A type that represents an entity struct.
typedef struct Entity Entity;
//A type that represents an entity action.
typedef struct EntityAction EntityAction;
//A type that represents an entity event.
typedef struct EntityEvent EntityEvent;

//This union is made so that homogenous EntityAction structs can be grouped in an array (therefore being iterable) but still can be accessed by keywords.
extern const union EntityActions
{
    struct ActionsGroup
    {
        EntityAction Punch, Kick, Throw, MagicAtk, Block;
    } S;
    EntityAction All[ACTIONCOUNT];
} EntityActions;

//Entity constructor struct that acts as a namespace to contain both constructor functions.
extern const struct EntityConstructor
{
    Entity *(*CreatePlayer)(char name[], char pronoun[]);
    Entity *(*CreateBot)(char name[], char pronoun[], int level);
} EntityConstructor;