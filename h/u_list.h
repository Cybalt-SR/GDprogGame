/*
This is a linked list element.
It is a struct with a pointer to a generic address which acts as its value.
Has references to the next and previous elements, therefore linking a list together.
Can be defined as a List Head which has implications regarding the logic that surrounds this struct.
*/
typedef struct ListElement
{
    void *value;
    int isListHead;
    struct ListElement *prev;
    struct ListElement *next;
} *ListElement;

/*
The method namespace for the differing functions that surrounds the ListElement struct.
*/
extern const struct List
{
    ListElement (*CreateList)(void);
    void (*DisposeList)(ListElement value);
    void (*RemoveElement)(ListElement value);
    void (*Add)(ListElement list, void * elementValue);
    void (*GetTotal)(ListElement list, int *total);
    void (*UpdateTick)(ListElement list);
} List;

/*
Element value types are the different types that are meant to be pointed to within the *value pointer of the ListElement struct.
This can point to other variable types, yes, but it is better if it is confined to the value types it is meant to hold.
*/

//The element value type for the modifying of an entity's def (or other stats in case the program scope is scaled).
typedef struct ModifierElementValue
{
    int modifier, turns_left;
} *ModifierElementValue;

//This is the namespace for the different constructors for the element value type structs
extern const struct Element{
    ModifierElementValue (*GetModifier)(int mod, int dur);
}Element;