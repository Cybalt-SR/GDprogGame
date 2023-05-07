#include "h/u_stdio.h"

static char *GetState(MovieSlotData slot)
{
    if(slot->onhold){
        return "ONHOLD";
    }else if (slot->reserved){
        return "RESERD";
    }
    else{
        return "USED";
    }
}

static void DisplaySeats(MovieSlotData slotData)
{
    Print("==SCREEN==\n\n", Colors.Reset);
    Print("Lower Box\n", Colors.Cyan);
    for (int i = 0; i < 30; i++)
    {
        if (i % 10 == 0 && i > 0)
            Print("\n", Colors.Reset);
        Print("%c", Colors.Reset, slotData->LowerBox[i]);
    }
    Print("\nUpper Box\n", Colors.Cyan);
    for (int i = 0; i < 20; i++)
    {
        if (i % 10 == 0 && i > 0)
            Print("\n", Colors.Reset);
        Print("%c", Colors.Reset, slotData->UpperBox[i]);
    }
    Print("\n\n", Colors.Reset);
}

static void DisplaySchedule(CinemaHall hall)
{
    Print("SLOT  |", Colors.Cyan);

    for (size_t day = 0; day < 5; day++)
    {
        Print("  %i   |", Colors.Cyan, day);
    }

    Print("\n", Colors.Cyan);

    for (size_t slot = 0; slot < 3; slot++)
    {
        switch (slot)
        {
        case 0:
            Print("1000  |", Colors.Cyan);
            break;
        case 1:
            Print("1300  |", Colors.Cyan);
            break;
        case 2:
            Print("1600  |", Colors.Cyan);
            break;
        }

        for (size_t day = 0; day < 5; day++)
        {
            Print(GetState(hall->schedule[day]->slots[slot]), Colors.Reset);
            Print("|", Colors.Cyan);
        }

        Print("\n", Colors.Cyan);
    }
}

static void Modify(CinemaHall hall)
{
}

static CinemaHall Create()
{
    CinemaHall hall = (CinemaHall)uMemAlloc(sizeof(struct CinemaHall));

    hall->schedule = (CinemaScheduleData *)uMemAlloc(sizeof(struct dataSlots) * 5);
    for (int day = 0; day < 5; day++)
    {
        hall->schedule[day] = (CinemaScheduleData)uMemAlloc(sizeof(struct dataSlots));

        for (int slot = 0; slot < 3; slot++)
        {
            hall->schedule[day]->slots[slot] = (MovieSlotData)uMemAlloc(sizeof(struct MovieSlotData));

            hall->schedule[day]->slots[slot]->LowerBox = (char *)uMemAlloc(sizeof(char) * 30);
            for (int seat = 0; seat < 30; seat++)
            {
                hall->schedule[day]->slots[slot]->LowerBox[seat] = 'X';
            }

            hall->schedule[day]->slots[slot]->UpperBox = (char *)uMemAlloc(sizeof(char) * 20);
            for (int seat = 0; seat < 30; seat++)
            {
                hall->schedule[day]->slots[slot]->UpperBox[seat] = 'X';
            }
        }
    }

    hall->seatingArr = 'R';

    return hall;
}

const struct Cinema Cinema = {
    .GetState = &GetState,
    .DisplaySchedule = &DisplaySchedule,
    .Modify = &Modify,
    .Create = &Create};