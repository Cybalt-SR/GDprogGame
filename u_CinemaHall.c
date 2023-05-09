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

static int AskMovie(MovieDatabase database, char *question)
{
    DisplayMovieDatabase(database);
    int id = AskInt(question);
    id = Max(id, 0);
    id = Min(id, database->length - 1);
    return id;
}

static MovieDatabase CreateMovieDatabase()
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
    int actionInt = AskMenu(
        "Actions : ", "Action to do : ", 3, Colors.Cyan, Colors.Yellow,
        "Edit existing movie",
        "Add movie",
        "Remove movie");

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

static char *GetState(MovieSlotData slot, MovieDatabase database)
{
    if (slot->onhold)
    {
        return "On Hold";
    }
    else if (slot->reserved)
    {
        char *state = (char *)uMemAlloc(sizeof(char *) * 100);
        strcpy(state, "Reserved for ");
        strcat(state, database->Movies[slot->movieID]->title);
        return state;
    }
    else if (slot->movieID > -1)
    {
        char *state = (char *)uMemAlloc(sizeof(char *) * 100);
        strcpy(state, "Used for ");
        strcat(state, database->Movies[slot->movieID]->title);
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

static void ResetSeating(MovieSlotData slotdata)
{
    for (size_t i = 0; i < 30; i++)
    {
        slotdata->LowerBox[i]->buyer = NULL;
    }

    for (size_t i = 0; i < 20; i++)
    {
        slotdata->UpperBox[i]->buyer = NULL;
    }
}

static int ValidateTicketBuy(MovieSlotData slotdata, char *name)
{
    int ticketsBought = 0;

    for (size_t i = 0; i < 30; i++)
    {
        if (strcmp(slotdata->LowerBox[i]->buyer, name) == 0)
        {
            ticketsBought++;
        }
    }
    for (size_t i = 0; i < 20; i++)
    {
        if (strcmp(slotdata->UpperBox[i]->buyer, name) == 0)
        {
            ticketsBought++;
        }
    }

    if (ticketsBought >= 5)
    {
        return 0;
    }
    else
    {
        return 1;
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
            char *state = GetState(hall->schedule[day]->slots[slot], moviedatabase);
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

        char *state = GetState(schedEditing->slots[slot], moviedatabase);
        Print(state, Colors.Reset);
        Print("\n", Colors.Cyan);
        free(state);
    }

    Print("[Warning: Changing any info regarding a slot will cancel all ticket sales on that slot]\n", Colors.Red);
    int dayAction = AskMenu(
        "Actions:", "Action to do : ", 4, Colors.Cyan, Colors.Yellow,
        "Reserve for Premiere",
        "Schedule Maintenance",
        "Slot a Movie",
        "Buy tickets for a slot");

    if (dayAction == 1)
    {
        if (schedEditing->slots[0]->reserved == 1)
        {
            Print("Premiere already scheduled, successfully unscheduled.\n", Colors.Green);

            for (size_t slot = 0; slot < 3; slot++)
            {
                ResetSeating(schedEditing->slots[slot]);
                schedEditing->slots[slot]->onhold = 0;
                schedEditing->slots[slot]->reserved = 0;
                schedEditing->slots[slot]->movieID = -1;
            }
        }
        else
        {
            int movie = AskMovie(moviedatabase, "Input movie ID to premiere : ");

            if (movie >= 0)
            {
                for (size_t slot = 0; slot < 3; slot++)
                {
                    ResetSeating(schedEditing->slots[slot]);
                    schedEditing->slots[slot]->onhold = 0;
                    schedEditing->slots[slot]->reserved = 1;
                    schedEditing->slots[slot]->movieID = movie;
                }
            }
        }
    }
    else if (dayAction == 2)
    {
        for (size_t slot = 0; slot < 3; slot++)
        {
            ResetSeating(schedEditing->slots[slot]);
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
        if (schedEditing->slots[0]->onhold == 1)
        {
            Print("You cannot schedule a movie in a day scheduled for maintenance.\n", Colors.Red);
        }
        else if (schedEditing->slots[0]->reserved == 1)
        {
            Print("You cannot schedule a movie in a day reserved for a premiere.\n", Colors.Red);
        }
        else
        {
            int slotIndex = AskInt("Input slot index : ");
            int movie = AskMovie(moviedatabase, "Input movie ID to slot : ");

            if (movie >= 0)
                schedEditing->slots[slotIndex]->movieID = movie;
        }
    }
    else if (dayAction == 4)
    {
        if (schedEditing->slots[0]->onhold == 1)
        {
            Print("You cannot buy tickets for a day scheduled for maintenance.\n", Colors.Red);
        }
        else
        {
            int slotIndex = AskInt("Input slot index : ");
            MovieSlotData slotToBuy = schedEditing->slots[slotIndex];

            if (slotToBuy->movieID == -1)
            {
                Print("You cannot buy tickets for a time slot with no movie scheduled.\n", Colors.Red);
            }
            else
            {
                ShowMovieSlotSeating(slotToBuy);

                int row = AskInt("Input seat row # : ");
                int col = AskInt("Input seat col # : ");
                int isUpperBox = row >= 3;
                int seatId = ((row % 3) * 10) + col;

                SeatData seatToBuy = isUpperBox ? slotToBuy->UpperBox[seatId] : slotToBuy->LowerBox[seatId];

                if (seatToBuy->buyer != NULL)
                {
                    Print("This seat is already occupied by %s.\n", Colors.Red, seatToBuy->buyer);
                }
                else
                {
                    Print("Buy R%iC%i in day %i slot %i for %i", Colors.Cyan, row, col, editDay, slotIndex, seatToBuy->price);
                    char *confirmation = AskString("? : ");
                    if (strcmp(confirmation, "Y") == 0 || strcmp(confirmation, "y") == 0)
                    {
                        char *buyerName = AskString("Please enter your name for the receipt : ");

                        if (ValidateTicketBuy(slotToBuy, buyerName))
                        {
                            free(seatToBuy->buyer);
                            seatToBuy->buyer = (char *)uMemAlloc(sizeof(char *) * 100);
                            strcpy(seatToBuy->buyer, buyerName);
                        }
                        else
                        {
                            Print("You cannot buy more than 5 tickets in a slot.\n", Colors.Red, seatToBuy->buyer);
                        }
                    }
                }
            }
        }
    }
}

static CinemaHallData CreateCinemaHallData()
{
    CinemaHallData hall = (CinemaHallData)uMemAlloc(sizeof(struct CinemaHallData));

    hall->schedule = (CinemaScheduleData *)uMemAlloc(sizeof(struct dataSlots *) * 5);
    for (int day = 0; day < 5; day++)
    {
        hall->schedule[day] = (CinemaScheduleData)uMemAlloc(sizeof(struct dataSlots));

        for (int slot = 0; slot < 3; slot++)
        {
            hall->schedule[day]->slots[slot] = (MovieSlotData)uMemAlloc(sizeof(struct MovieSlotData));
            hall->schedule[day]->slots[slot]->seatingArr = 'R';
            hall->schedule[day]->slots[slot]->onhold = 0;
            hall->schedule[day]->slots[slot]->reserved = 0;
            hall->schedule[day]->slots[slot]->movieID = -1;

            hall->schedule[day]->slots[slot]->LowerBox = (SeatData *)uMemAlloc(sizeof(struct SeatData *) * 30);
            for (int seat = 0; seat < 30; seat++)
            {
                hall->schedule[day]->slots[slot]->LowerBox[seat] = (SeatData)uMemAlloc(sizeof(struct SeatData));
                hall->schedule[day]->slots[slot]->LowerBox[seat]->buyer = NULL;
                hall->schedule[day]->slots[slot]->LowerBox[seat]->price = 100;
            }

            hall->schedule[day]->slots[slot]->UpperBox = (SeatData *)uMemAlloc(sizeof(struct SeatData *) * 20);
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

static void SerializeHallData(char *path, CinemaHallData halldata)
{
    FILE *fptr = fopen(path, "w");

    for (size_t day = 0; day < 5; day++)
    {
        CinemaScheduleData daydata = halldata->schedule[day];

        for (size_t slot = 0; slot < 3; slot++)
        {
            MovieSlotData slotdata = daydata->slots[slot];

            fprintf(fptr, "d%is%imovie: %i\n", day, slot, slotdata->movieID);
            fprintf(fptr, "d%is%ionhold: %i\n", day, slot, slotdata->onhold);
            fprintf(fptr, "d%is%ireserved: %i\n", day, slot, slotdata->reserved);
            fprintf(fptr, "d%is%iseatingArr: %c\n", day, slot, slotdata->seatingArr);

            for (size_t i = 0; i < 30; i++)
            {
                fprintf(fptr, "d%is%il%ibuyer: %s\n", day, slot, i, slotdata->LowerBox[i]->buyer);
            }
            for (size_t i = 0; i < 20; i++)
            {
                fprintf(fptr, "d%is%iu%ibuyer: %s\n", day, slot, i, slotdata->UpperBox[i]->buyer);
            }
        }
    }
    fclose(fptr);
}

static CinemaHallData LoadHallData(char *path)
{
    CinemaHallData hall = CreateCinemaHallData();
    FILE *fptr = fopen(path, "r");

    char curChar = ' ';
    char curCharStr[2];
    curCharStr[1] = '\0';
    char mostRecentIdentifier;
    char *curWord = (char *)uMemAlloc(sizeof(char) * 100);
    strcpy(curWord, "");

    CinemaScheduleData day;
    MovieSlotData slot;
    SeatData seat;

    int terminate = 0;
    while (terminate == 0)
    {
        curChar = fgetc(fptr);

        if (curChar == EOF)
        {
            terminate = 1;
        }
        else
        {
            curCharStr[0] = curChar;
            strcat(curWord, curCharStr);

            if ((curChar == 'd' || curChar == 's' || curChar == 'l' || curChar == 'u') && strlen(curWord) <= 1)
            {
                int index;
                fscanf(fptr, "%i", &index);

                switch (curChar)
                {
                case 'd':
                    day = hall->schedule[index];
                    break;
                case 's':
                    slot = day->slots[index];
                    break;
                case 'l':
                    seat = slot->LowerBox[index];
                    break;
                case 'u':
                    seat = slot->UpperBox[index];
                    break;
                }
                mostRecentIdentifier = curChar;

                strcpy(curWord, "");
            }

            if (curChar == '\n')
            {
                strcpy(curWord, "");
            }
            if (mostRecentIdentifier == 's')
            {
                if (strcmp(curWord, "movie") == 0)
                {
                    int value;
                    fseek(fptr, 2, SEEK_CUR);
                    fscanf(fptr, "%i", &value);
                    slot->movieID = value;
                    strcpy(curWord, "");
                }
                else if (strcmp(curWord, "onhold") == 0)
                {
                    int value;
                    fseek(fptr, 2, SEEK_CUR);
                    fscanf(fptr, "%i", &value);
                    slot->onhold = value;
                    strcpy(curWord, "");
                }
                else if (strcmp(curWord, "reserved") == 0)
                {
                    int value;
                    fseek(fptr, 2, SEEK_CUR);
                    fscanf(fptr, "%i", &value);
                    slot->reserved = value;
                    strcpy(curWord, "");
                }
                else if (strcmp(curWord, "seatingArr") == 0)
                {
                    char value;
                    fseek(fptr, 2, SEEK_CUR);
                    fscanf(fptr, "%c", &value);
                    slot->seatingArr = value;
                    strcpy(curWord, "");
                }
            }
            else if (mostRecentIdentifier == 'l' || mostRecentIdentifier == 'u')
            {
                if (strcmp(curWord, "buyer") == 0)
                {
                    char *value = (char *)uMemAlloc(sizeof(char *) * 100);
                    fseek(fptr, 2, SEEK_CUR);
                    fscanf(fptr, "%s", value);

                    if (strcmp(value, "(null)") == 0)
                    {
                        seat->buyer = NULL;
                    }
                    else
                    {
                        seat->buyer = value;
                    }
                    strcpy(curWord, "");
                }
            }
        }
    }

    free(curWord);
    fclose(fptr);
    return hall;
}

static MovieDatabase LoadMovieDatabase(char *path)
{
    MovieDatabase hall = CreateMovieDatabase();
    FILE *fptr = fopen(path, "r");

    char curChar = ' ';
    char curCharStr[2];
    curCharStr[1] = '\0';
    char mostRecentIdentifier;
    char *curWord = (char *)uMemAlloc(sizeof(char) * 100);
    strcpy(curWord, "");

    MovieData data;
    int terminate = 0;
    while (terminate == 0)
    {
        curChar = fgetc(fptr);
        if (curChar == EOF)
        {
            terminate = 1;
        }
        else
        {
            curCharStr[0] = curChar;
            strcat(curWord, curCharStr);

            if (strcmp(curWord, "dlength") == 0)
            {
                int value;
                fseek(fptr, 2, SEEK_CUR);
                fscanf(fptr, "%i", &value);
                hall->length = value;
                strcpy(curWord, "");

                hall->Movies = (MovieData *)uMemAlloc(sizeof(struct MovieData) * hall->length);

                for (size_t i = 0; i < hall->length; i++)
                {
                    hall->Movies[i] = (MovieData)uMemAlloc(sizeof(struct MovieData));
                }
            }

            if (curChar == 'i' && strlen(curWord) <= 1)
            {
                int index;
                fscanf(fptr, "%i", &index);
                data = hall->Movies[index];
                mostRecentIdentifier = curChar;

                strcpy(curWord, "");
            }

            if (curChar == '\n')
            {
                strcpy(curWord, "");
            }
            if (mostRecentIdentifier == 'i')
            {
                if (strcmp(curWord, "title") == 0)
                {
                    data->title = (char *)uMemAlloc(sizeof(char *) * 100);
                    fseek(fptr, 2, SEEK_CUR);
                    fscanf(fptr, "%s", data->title);
                    strcpy(curWord, "");
                }
                else if (strcmp(curWord, "rating") == 0)
                {
                    int value;
                    fseek(fptr, 2, SEEK_CUR);
                    fscanf(fptr, "%i", &value);
                    data->rating = value;
                    strcpy(curWord, "");
                }
                else if (strcmp(curWord, "summary") == 0)
                {
                    data->summary = (char *)uMemAlloc(sizeof(char *) * 100);
                    fseek(fptr, 2, SEEK_CUR);
                    fscanf(fptr, "%s", data->summary);
                    strcpy(curWord, "");
                }
                else if (strcmp(curWord, "actors") == 0)
                {
                    data->actors = (char *)uMemAlloc(sizeof(char *) * 100);
                    fseek(fptr, 2, SEEK_CUR);
                    fscanf(fptr, "%s", data->actors);
                    strcpy(curWord, "");
                }
            }
        }
    }

    free(curWord);
    fclose(fptr);
    return hall;
}

static void SerializeMovieDatabase(char *path, MovieDatabase database)
{
    FILE *fptr = fopen(path, "w");

    fprintf(fptr, "dlength: %i\n", database->length);

    for (size_t index = 0; index < database->length; index++)
    {
        MovieData data = database->Movies[index];

        fprintf(fptr, "i%ititle: %s\n", index, data->title);
        fprintf(fptr, "i%irating: %i\n", index, data->rating);
        fprintf(fptr, "i%isummary: %s\n", index, data->summary);
        fprintf(fptr, "i%iactors: %s\n", index, data->actors);
    }
    fclose(fptr);
}

const struct Movie Movie = {
    .EditDatabase = &EditMovieDatabase,
    .Create = &CreateMovieDatabase,
    .Load = &LoadMovieDatabase,
    .SerializeMovieDatabase = &SerializeMovieDatabase};

const struct Cinema Cinema = {
    .DisplayMovieHallSchedule = &DisplayMovieHallSchedule,
    .Create = &CreateCinemaHallData,
    .SerializeHallData = &SerializeHallData,
    .Load = &LoadHallData};