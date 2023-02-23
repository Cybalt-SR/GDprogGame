#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "h/EntityList.h"

EntityListElement CreateList(int size, ...)
{
    va_list list;
    va_start(list, size);

    EntityListElement First = NULL;
    EntityListElement prev = NULL;

    for (size_t i = 0; i < size; i++)
    {
        EntityListElement temp = (EntityListElement)malloc(sizeof(struct EntityListElement));
        temp->value = va_arg(list, EntityListElementValue);

        if (First == NULL)
        {
            First = temp;
        }

        temp->prev = prev;
        temp->next = NULL;

        if (prev != NULL)
        {
            prev->next = temp;
        }

        prev = temp;
    }

    va_end(list);

    return First;
}
void DisposeList(EntityListElement value)
{
    if (value->prev != NULL)
    {
        DisposeList(value->prev);
    }
    if (value->next != NULL)
    {
        DisposeList(value->next);
    }

    free(value);
}
void RemoveElement(EntityListElement value)
{
    if (value->prev != NULL)
    {
        value->prev->next = value->next;
    }
    if (value->next != NULL)
    {
        value->next->prev = value->prev;
    }

    free(value);
}
void Add(EntityListElement list, EntityListElementValue value)
{
    EntityListElement temp = (EntityListElement)malloc(sizeof(struct EntityListElement));
    temp->value = value;

    temp->prev = NULL;
    temp->next = NULL;

    if (list == NULL)
    {
        list = temp;
    }
    else
    {
        EntityListElement end = list;

        while (end->next != NULL)
        {
            end = end->next;
        }

        temp->prev = end;
        end->next = temp;
    }
}

void GetTotal(EntityListElement list, int *total)
{
    if (list->prev != NULL)
    {
        GetTotal(list->prev, total);
    }
    if (list->next != NULL)
    {
        GetTotal(list->next, total);
    }

    *total += list->value->modifier;
}
void UpdateTick(EntityListElement list)
{
    if (list->prev != NULL)
    {
        UpdateTick(list->prev);
    }
    if (list->next != NULL)
    {
        UpdateTick(list->next);
    }

    list->value->turns_left -= 1;
}

const struct EntityList EntityList = {
    .CreateList = &CreateList,
    .DisposeList = &DisposeList,
    .RemoveElement = &RemoveElement,
    .Add = &Add,
    .GetTotal = &GetTotal,
    .UpdateTick = &UpdateTick};