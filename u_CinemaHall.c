#include "h/u_stdio.h"

static MovieData AskMovie()
{
}

static char *GetState(MovieSlotData slot)
{
    if (slot->onhold)
    {
        return "H";
    }
    else if (slot->reserved)
    {
        return "R";
    }
    else if (slot->movie != NULL)
    {
        return "U";
    }
    else
    {
        return "V";
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
            Print("%-6s", Colors.Reset, GetState(hall->schedule[day]->slots[slot]));
            Print("|", Colors.Cyan);
        }

        Print("\n", Colors.Cyan);
    }

    int editDay = AskInt("Enter a day you want to edit [0-4] : ");
    Print("You are editing day %i\n", Colors.Yellow, editDay);

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

        Print(GetState(hall->schedule[editDay]->slots[slot]), Colors.Reset);
        Print("\n", Colors.Cyan);
    }

    Print("Actions: \n", Colors.Cyan);
    Print("[1] Reserve for Premiere\n", Colors.Cyan);
    Print("[2] Schedule Maintenance\n", Colors.Cyan);
    Print("[3] Slot a Movie\n", Colors.Cyan);
    int dayAction = AskInt("Which action : ");

    if (dayAction == 1)
    {
    }
    else if (dayAction == 2)
    {
        for (size_t slot = 0; slot < 3; slot++)
        {
            hall->schedule[editDay]->slots[slot]->onhold = 1;
        }

        Print("Successfully scheduled maintenance.\n", Colors.Green);
    }
    else if (dayAction == 3)
    {
    }
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
            hall->schedule[day]->slots[slot]->onhold = 0;
            hall->schedule[day]->slots[slot]->reserved = 0;
            hall->schedule[day]->slots[slot]->movie = NULL;

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
    .DisplaySchedule = &DisplaySchedule,
    .Create = &Create};