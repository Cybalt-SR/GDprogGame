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
            printf("|\n stiched %u to %u |", element->next, element->prev);

        element->prev->next = element->next;
    }
    if (element->next != NULL)
    {
        if (isDebugging)
            printf("|\n stiched %u to %u |", element->prev, element->next);

        element->next->prev = element->prev;
    }

    free(element->value);
    free(element);
}
static void Add(EntityListElement list, int modValue, int duration)
{
    EntityListElement temp = (EntityListElement)malloc(sizeof(EntityListElement));
    EntityListElementValue value = (EntityListElementValue)malloc(sizeof(EntityListElementValue));
    value->modifier = modValue;
    value->turns_left = duration;

    temp->value = value;
    temp->isListHead = 0;
    temp->prev = NULL;
    temp->next = NULL;

    if (isDebugging)
        printf("\n| Attaching %u to %u |", temp, list);

    EntityListElement end = list;

    while (end->next != NULL)
    {
        end = end->next;
    }

    temp->prev = end;
    end->next = temp;
}

static void GetTotal(EntityListElement list, int *total)
{
    if (isDebugging)
        printf("\n| Tried to add to %i from %u |", *total, list);

    if (list != NULL)
    {
        GetTotal(list->next, total);
        *total += list->value->modifier;
    }

    if (isDebugging)
        printf("| New total after add: %i |", *total);
}
static void UpdateTick(EntityListElement list)
{
    if (list != NULL)
    {
        if (isDebugging)
            printf("\n| Updating %u |", list);

        UpdateTick(list->next);

        list->value->turns_left -= 1;

        if (list->value->turns_left == 0 && list->isListHead != 1)
        {
            if (isDebugging)
                printf("\n| Deleting %u |", list);

            RemoveElement(list);
        }
    }

    if (isDebugging)
        printf("\n");
}
static EntityListElement CreateList()
{
    EntityListElement temp = (EntityListElement)malloc(sizeof(EntityListElement));
    EntityListElementValue value = (EntityListElementValue)malloc(sizeof(EntityListElementValue));
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