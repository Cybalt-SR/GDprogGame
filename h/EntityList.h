typedef struct EntityListElementValue
{
    int modifier, turns_left;
} *EntityListElementValue;

typedef struct EntityListElement
{
    EntityListElementValue value;
    int isListHead;
    struct EntityListElement *prev;
    struct EntityListElement *next;
} *EntityListElement;

extern const struct EntityList
{
    EntityListElement (*CreateList)(void);
    void (*DisposeList)(EntityListElement value);
    void (*RemoveElement)(EntityListElement value);
    void (*Add)(EntityListElement list, int modValue, int duration);
    void (*GetTotal)(EntityListElement list, int *total);
    void (*UpdateTick)(EntityListElement list);
} EntityList;