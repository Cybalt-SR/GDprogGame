#include "h/u_stdio.h"

static void DisposeList(ListElement value)
{
    if (value->next != NULL)
    {
        DisposeList(value->next);
    }

    free(value);
}
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

static void GetTotal(ListElement list, int *total)
{
    if (list != NULL)
    {
        printDebugText("| Tried to add to %i from %u |\n", *total, list);

        GetTotal(list->next, total);

        if ((ModifierElementValue)list->value != NULL)
            *total += ((ModifierElementValue)list->value)->modifier;
    }
    else
    {
        printDebugText("| Tried to add to %i from NULL |\n", *total);
    }
}
static void UpdateTick(ListElement list)
{
    if (list != NULL)
    {
        printDebugText("| Updating %u |", list);

        UpdateTick(list->next);

        if (list->isListHead != 1)
        {
            ((ModifierElementValue)list->value)->turns_left -= 1;

            if (((ModifierElementValue)list->value)->turns_left == 0)
            {
                printDebugText("| Deleting %u |", list);

                RemoveElement(list);
            }
        }
    }

    printDebugText("\n");
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
    .Add = &Add,
    .GetTotal = &GetTotal,
    .UpdateTick = &UpdateTick};

// Element Value Constructors

static ModifierElementValue GetModifierValue(int modValue, int duration)
{
    ModifierElementValue value = (ModifierElementValue)uMemAlloc(sizeof(struct ModifierElementValue));
    value->modifier = modValue;
    value->turns_left = duration;
}

const struct Element Element = {
    .GetModifier = &GetModifierValue};