#include "h/Util.h"
#include "h/EntityList.h"

static void DisposeList(EntityListElement value)
{
    if (value->next != NULL)
    {
        DisposeList(value->next);
    }

    free(value);
}
static void RemoveElement(EntityListElement element)
{
    if (element->prev != NULL)
    {
        if (isDebugging)
            printDebugText("| stiched %u to %u |", element->next, element->prev);

        element->prev->next = element->next;
    }
    if (element->next != NULL)
    {
        if (isDebugging)
            printDebugText("| stiched %u to %u |", element->prev, element->next);

        element->next->prev = element->prev;
    }

    if (isDebugging)
        printDebugText("\n");

    free(element);
}
static void Add(EntityListElement list, int modValue, int duration)
{
    EntityListElement temp = (EntityListElement)malloc(sizeof(struct EntityListElement));
    EntityListElementValue value = (EntityListElementValue)malloc(sizeof(struct EntityListElementValue));
    value->modifier = modValue;
    value->turns_left = duration;

    temp->value = value;
    temp->isListHead = 0;
    temp->prev = NULL;
    temp->next = NULL;

    if (isDebugging)
        printDebugText("| Attaching %u to %u |\n", temp, list);

    EntityListElement end = list;

    while (end->next != NULL)
    {
        end = end->next;
    }

    end->next = temp;
    temp->prev = end;
}

static void GetTotal(EntityListElement list, int *total)
{
    if (list != NULL)
    {
        if (isDebugging)
            printDebugText("| Tried to add to %i from %u |\n", *total, list);

        GetTotal(list->next, total);
        *total += list->value->modifier;
    }
    else
    {
        if (isDebugging)
            printDebugText("| Tried to add to %i from NULL |\n", *total);
    }
}
static void UpdateTick(EntityListElement list)
{
    if (list != NULL)
    {
        if (isDebugging)
            printDebugText("| Updating %u |", list);

        UpdateTick(list->next);

        if (list->isListHead != 1)
        {
            list->value->turns_left -= 1;

            if (list->value->turns_left == 0)
            {
                if (isDebugging)
                    printDebugText("| Deleting %u |", list);

                RemoveElement(list);
            }
        }
    }

    if (isDebugging)
        printDebugText("\n");
}
static EntityListElement CreateList()
{
    EntityListElement temp = (EntityListElement)malloc(sizeof(struct EntityListElement));
    EntityListElementValue value = (EntityListElementValue)malloc(sizeof(struct EntityListElementValue));
    value->modifier = 0;
    value->turns_left = 0;

    temp->isListHead = 1;
    temp->value = value;
    temp->next = NULL;
    temp->prev = NULL;

    return temp;
}

const struct EntityList EntityList = {
    .CreateList = &CreateList,
    .DisposeList = &DisposeList,
    .RemoveElement = &RemoveElement,
    .Add = &Add,
    .GetTotal = &GetTotal,
    .UpdateTick = &UpdateTick};