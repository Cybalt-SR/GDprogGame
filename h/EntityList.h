typedef struct ListElement
{
    void *value;
    int isListHead;
    struct ListElement *prev;
    struct ListElement *next;
} *ListElement;

extern const struct EntityList
{
    ListElement (*CreateList)(void);
    void (*DisposeList)(ListElement value);
    void (*RemoveElement)(ListElement value);
    void (*Add)(ListElement list, int modValue, int duration);
    void (*GetTotal)(ListElement list, int *total);
    void (*UpdateTick)(ListElement list);
} EntityList;

//Element Types

typedef struct ModifierElementValue
{
    int modifier, turns_left;
} *ModifierElementValue;