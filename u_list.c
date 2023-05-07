#include "h/u_stdio.h"

/*
Use: To completely dispose a list which uses the list head of a linked list.
*/
static void DisposeList(ListElement value)
{
    if (value->next != NULL)
    {
        DisposeList(value->next);
    }

    free(value);
}
/*
Use: Removes an element and stiches the previous and next element.
*/
static void RemoveElement(ListElement element)
{
    if (element->prev != NULL)
    {
        printDebugText("| stiched %u to %u |", element->next, element->prev);

        element->prev->next = element->next;
    }
    if (element->next != NULL)
    {
        printDebugText("| stiched %u to %u |", element->prev, element->next);

        element->next->prev = element->prev;
    }

    printDebugText("\n");

    free(element);
}
static void Add(ListElement list, void *elementValue)
{
    ListElement temp = (ListElement)uMemAlloc(sizeof(struct ListElement));

    temp->value = elementValue;
    temp->isListHead = 0;
    temp->prev = NULL;
    temp->next = NULL;

    printDebugText("| Attaching %u to %u |\n", temp, list);

    ListElement end = list;

    while (end->next != NULL)
    {
        end = end->next;
    }

    end->next = temp;
    temp->prev = end;
}
static ListElement CreateList()
{
    ListElement temp = (ListElement)uMemAlloc(sizeof(struct ListElement));

    temp->isListHead = 1;
    temp->value = NULL;
    temp->next = NULL;
    temp->prev = NULL;

    return temp;
}

const struct List List = {
    .CreateList = &CreateList,
    .DisposeList = &DisposeList,
    .RemoveElement = &RemoveElement,
    .Add = &Add};
