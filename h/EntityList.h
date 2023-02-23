typedef struct EntityListElementValue
{
    int modifier, turns_left;
} *EntityListElementValue;

typedef struct EntityListElement
{
    EntityListElementValue value;
    struct EntityListElement *prev;
    struct EntityListElement *next;
} *EntityListElement;

extern const struct EntityList
{
    EntityListElement (*CreateList)(int size, ...);
    void (*DisposeList)(EntityListElement value);
    void (*RemoveElement)(EntityListElement value);
    void (*Add)(EntityListElement list, EntityListElementValue value);
    void (*GetTotal)(EntityListElement list, int *total);
    void (*UpdateTick)(EntityListElement list);
} EntityList;