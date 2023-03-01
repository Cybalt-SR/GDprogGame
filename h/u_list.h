typedef struct ListElement
{
    void *value;
    int isListHead;
    struct ListElement *prev;
    struct ListElement *next;
} *ListElement;

extern const struct List
{
    ListElement (*CreateList)(void);
    void (*DisposeList)(ListElement value);
    void (*RemoveElement)(ListElement value);
    void (*Add)(ListElement list, void * elementValue);
    void (*GetTotal)(ListElement list, int *total);
    void (*UpdateTick)(ListElement list);
} List;

//Element Types

typedef struct ModifierElementValue
{
    int modifier, turns_left;
} *ModifierElementValue;

extern const struct Element{
    ModifierElementValue (*GetModifier)(int mod, int dur);
}Element;