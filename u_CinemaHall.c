#include "h/u_stdio.h"

static void DisplayMovieDatabase(MovieDatabase database)
{
    Print("Movies in database : \n", Colors.Cyan);
    for (size_t i = 0; i < database->length; i++)
    {
        Print("=== Movie ID[%i] : ===\n", Colors.Cyan, i);
        Print("Title : %s\n", Colors.Cyan, database->Movies[i]->title);
        Print("Rating : %i\n", Colors.Cyan, database->Movies[i]->rating);
        Print("Summary : %s\n", Colors.Cyan, database->Movies[i]->summary);
        Print("Actors : %s\n", Colors.Cyan, database->Movies[i]->actors);
    }
}

static MovieData AskMovie(MovieDatabase database, char *question)
{
    DisplayMovieDatabase(database);
    int movieIndex = AskInt(question);
    return database->Movies[movieIndex];
}

static MovieDatabase CreateMovieData()
{
    MovieDatabase database = (MovieDatabase)uMemAlloc(sizeof(struct MovieDatabase));

    database->length = 0;
    database->Movies = (MovieData *)uMemAlloc(sizeof(struct MovieData) * 0);

    return database;
}

static void EditMovieDatabase(MovieDatabase database)
{
    DisplayMovieDatabase(database);
    Print("\n", Colors.Reset);
    Print("Actions : \n", Colors.Cyan);
    Print("[1] Edit existing movie\n", Colors.Cyan);
    Print("[2] Add movie\n", Colors.Cyan);
    Print("[3] Remove movie\n", Colors.Cyan);
    int actionInt = AskInt("Action to do : ");

    if (actionInt == 1)
    {
        int movieIndex = AskInt("Input movie ID to edit : ");
        MovieData toEdit = database->Movies[movieIndex];

        toEdit->title = AskString("Input new title : ");
        toEdit->rating = AskInt("Input new rating : ");
        toEdit->summary = AskString("Input new summary : ");
        toEdit->actors = AskString("Input new actors : ");
    }
    else if (actionInt == 2)
    {
        MovieData newData = (MovieData)uMemAlloc(sizeof(struct MovieData));

        newData->title = AskString("Input title : ");
        newData->rating = AskInt("Input rating : ");
        newData->summary = AskString("Input summary : ");
        newData->actors = AskString("Input actors : ");

        database->length++;
        database->Movies = (MovieData *)realloc(database->Movies, sizeof(struct MovieData) * database->length);

        database->Movies[database->length - 1] = newData;
    }
    else if (actionInt == 3)
    {
        int movieIndex = AskInt("Input movie ID to nullify : ");
        MovieData toEdit = database->Movies[movieIndex];

        toEdit->title = "NULL";
        toEdit->rating = 0;
        toEdit->summary = "NULL";
        toEdit->actors = "NULL";
    }
}

static char *GetState(MovieSlotData slot)
{
    if (slot->onhold)
    {
        return "On Hold";
    }
    else if (slot->reserved)
    {
        char *state = (char *)uMemAlloc(sizeof(char *) * 100);
        strcpy(state, "Reserved for ");
        strcat(state, slot->movie->title);
        return state;
    }
    else if (slot->movie != NULL)
    {
        char *state = (char *)uMemAlloc(sizeof(char *) * 100);
        strcpy(state, "Used for ");
        strcat(state, slot->movie->title);
        return state;
    }
    else
    {
        return "Vacant";
    }
}

static void PrintSeatingLabel(SeatData seatdata)
{
    if (seatdata->buyer != NULL)
    {
        Print(" X ", Colors.Red);
    }
    else
    {
        Print(" O ", Colors.Green);
    }
}

static void ShowMovieSlotSeating(MovieSlotData slotdata)
{
    Print("=============SCREEN=============\n\n", Colors.Reset);
    Print("            Lower Box           \n", Colors.Cyan);
    Print("   ", Colors.Cyan);
    for (int i = 0; i < 10; i++)
    {
        Print(" %i ", Colors.Cyan, i);
    }
    Print("\n", Colors.Reset);
    for (int i = 0; i < 30; i++)
    {
        if (i % 10 == 0)
        {
            if (i > 0)
                Print("\n", Colors.Reset);
            Print("%i  ", Colors.Cyan, i / 10);
        }
        PrintSeatingLabel(slotdata->LowerBox[i]);
    }
    Print("\n            Upper Box           \n", Colors.Cyan);
    for (int i = 0; i < 20; i++)
    {
        if (i % 10 == 0)
        {
            if (i > 0)
                Print("\n", Colors.Reset);
            Print("%i  ", Colors.Cyan, (i / 10) + 3);
        }
        PrintSeatingLabel(slotdata->UpperBox[i]);
    }
    Print("\n\n", Colors.Reset);
}

static void DisplayMovieHallSchedule(CinemaHallData hall, MovieDatabase moviedatabase)
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
            char *state = GetState(hall->schedule[day]->slots[slot]);
            Print("%-6.6s", Colors.Reset, state);
            Print("|", Colors.Cyan);
            free(state);
        }

        Print("\n", Colors.Cyan);
    }

    int editDay = AskInt("Enter a day you want to edit [0-4] : ");
    Print("You are editing day %i\n", Colors.Yellow, editDay);
    CinemaScheduleData schedEditing = hall->schedule[editDay];

    for (size_t slot = 0; slot < 3; slot++)
    {
        switch (slot)
        {
        case 0:
            Print("1000 [0] -> ", Colors.Cyan);
            break;
        case 1:
            Print("1300 [1] -> ", Colors.Cyan);
            break;
        case 2:
            Print("1600 [2] -> ", Colors.Cyan);
            break;
        }

        char *state = GetState(schedEditing->slots[slot]);
        Print(state, Colors.Reset);
        Print("\n", Colors.Cyan);
        free(state);
    }

    Print("[Warning: Changing any info regarding a slot will cancel all ticket sales on that slot]\n", Colors.Red);
    Print("Actions: \n", Colors.Cyan);
    Print("[1] Reserve for Premiere\n", Colors.Cyan);
    Print("[2] Schedule Maintenance\n", Colors.Cyan);
    Print("[3] Slot a Movie\n", Colors.Cyan);
    Print("[4] Buy tickets for a slot\n", Colors.Cyan);
    int dayAction = AskInt("Which action : ");

    if (dayAction == 1)
    {
        MovieData movie = AskMovie(moviedatabase, "Input movie ID to premiere : ");

        for (size_t slot = 0; slot < 3; slot++)
        {
            schedEditing->slots[slot]->onhold = 0;
            schedEditing->slots[slot]->reserved = 1;
            schedEditing->slots[slot]->movie = movie;
        }
    }
    else if (dayAction == 2)
    {
        for (size_t slot = 0; slot < 3; slot++)
        {
            schedEditing->slots[slot]->onhold = !schedEditing->slots[slot]->onhold;
        }

        if (schedEditing->slots[0]->onhold == 1)
        {
            Print("Successfully scheduled maintenance.\n", Colors.Green);
        }
        else
        {
            Print("Maintenance was already scheduled, successfully unscheduled.\n", Colors.Green);
        }
    }
    else if (dayAction == 3)
    {
        int slotIndex = AskInt("Input slot index : ");
        MovieData movie = AskMovie(moviedatabase, "Input movie ID to slot : ");

        schedEditing->slots[slotIndex]->movie = movie;
    }
    else if (dayAction == 4)
    {
        int slotIndex = AskInt("Input slot index : ");
        MovieSlotData slotToBuy = schedEditing->slots[slotIndex];

        ShowMovieSlotSeating(slotToBuy);

        int row = AskInt("Input seat row # : ");
        int col = AskInt("Input seat col # : ");
        int isUpperBox = row >= 3;
        int seatId = ((row % 3) * 10) + col;

        SeatData seatToBuy = isUpperBox ? slotToBuy->UpperBox[seatId] : slotToBuy->LowerBox[seatId];

        if (seatToBuy->buyer != NULL)
        {
            Print("This seat is already occupied by %s.", Colors.Red, seatToBuy->buyer);
        }
        else
        {
            Print("Buy R%iC%i for %i", Colors.Cyan, row, col);
            char *confirmation = AskString("? : ");
            if (confirmation == "Y" || confirmation == "y")
            {
                char *buyerName = AskString("Please enter your name for the receipt : ");
                free(seatToBuy->buyer);
                seatToBuy->buyer = (char *)uMemAlloc(sizeof(char *) * 100);
                strcpy(seatToBuy->buyer, buyerName);
            }
        }
    }
}

static CinemaHallData CreateCinemaHallData()
{
    CinemaHallData hall = (CinemaHallData)uMemAlloc(sizeof(struct CinemaHallData));

    hall->schedule = (CinemaScheduleData *)uMemAlloc(sizeof(struct dataSlots*) * 5);
    for (int day = 0; day < 5; day++)
    {
        hall->schedule[day] = (CinemaScheduleData)uMemAlloc(sizeof(struct dataSlots));

        for (int slot = 0; slot < 3; slot++)
        {
            hall->schedule[day]->slots[slot] = (MovieSlotData)uMemAlloc(sizeof(struct MovieSlotData));
            hall->schedule[day]->slots[slot]->seatingArr = 'R';
            hall->schedule[day]->slots[slot]->onhold = 0;
            hall->schedule[day]->slots[slot]->reserved = 0;
            hall->schedule[day]->slots[slot]->movie = NULL;

            hall->schedule[day]->slots[slot]->LowerBox = (SeatData *)uMemAlloc(sizeof(struct SeatData*) * 30);
            for (int seat = 0; seat < 30; seat++)
            {
                hall->schedule[day]->slots[slot]->LowerBox[seat] = (SeatData)uMemAlloc(sizeof(struct SeatData));
                hall->schedule[day]->slots[slot]->LowerBox[seat]->buyer = NULL;
                hall->schedule[day]->slots[slot]->LowerBox[seat]->price = 100;
            }

            hall->schedule[day]->slots[slot]->UpperBox = (SeatData *)uMemAlloc(sizeof(struct SeatData*) * 20);
            for (int seat = 0; seat < 20; seat++)
            {
                hall->schedule[day]->slots[slot]->UpperBox[seat] = (SeatData)uMemAlloc(sizeof(struct SeatData));
                hall->schedule[day]->slots[slot]->UpperBox[seat]->buyer = NULL;
                hall->schedule[day]->slots[slot]->UpperBox[seat]->price = 200;
            }
        }
    }

    return hall;
}

const struct Movie Movie = {
    .EditDatabase = &EditMovieDatabase,
    .Create = &CreateMovieData};

const struct Cinema Cinema = {
    .DisplayMovieHallSchedule = &DisplayMovieHallSchedule,
    .Create = &CreateCinemaHallData};