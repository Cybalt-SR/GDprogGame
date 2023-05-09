/*
NOTE: When compiling, make sure to include ALL THREE *.c files in the compilation inputs.
    This is so that the linker will be able to link prototypes from the *.h files to their respective definitions in the *.c files.

FILES NEEDED TO INCLUDE:
    Main.c
    u_CinemaHall.c
    u_list.c
*/

#include "h/u_stdio.h"

void DisplayHallReport(CinemaHallData hall, int hallID)
{
    Print("Report for Hall %i ========================\n", Colors.Yellow, hallID);

    Print("               |", Colors.Yellow, hallID);

    for (size_t day = 0; day < 5; day++)
    {
        Print(" Day %i |", Colors.Yellow, day);
    }
    Print("\n", Colors.Cyan);
    for (size_t slot = 0; slot < 3; slot++)
    {
        Print("[slot %i]\n", Colors.Yellow, slot);

        for (size_t attributes = 0; attributes < 3; attributes++)
        {
            switch (attributes)
            {
            case 0:
                Print("Tickets sold   |", Colors.Cyan);
                break;
            case 1:
                Print("Ticket revenue |", Colors.Cyan);
                break;
            case 2:
                Print("Vacant seats   |", Colors.Cyan);
                break;
            }

            for (size_t day = 0; day < 5; day++)
            {
                CinemaScheduleData daydata = hall->schedule[day];
                MovieSlotData slotdata = daydata->slots[slot];

                int ticketNumber = 0;
                int ticketProfit = 0;
                int vacantSeats = 50;

                for (size_t i = 0; i < 30; i++)
                {
                    if (slotdata->LowerBox[i]->buyer != NULL)
                    {
                        ticketNumber++;
                        vacantSeats--;
                        ticketProfit += slotdata->LowerBox[i]->price;
                    }
                }
                for (size_t i = 0; i < 20; i++)
                {
                    if (slotdata->UpperBox[i]->buyer != NULL)
                    {
                        ticketNumber++;
                        vacantSeats--;
                        ticketProfit += slotdata->UpperBox[i]->price;
                    }
                }

                int numToPrint;
                switch (attributes)
                {
                case 0:
                    numToPrint = ticketNumber;
                    break;
                case 1:
                    numToPrint = ticketProfit;
                    break;
                case 2:
                    numToPrint = vacantSeats;
                    break;
                }

                int length = snprintf(NULL, 0, "%d", numToPrint);
                char *numToPrintStr = malloc(length + 1);
                snprintf(numToPrintStr, length + 1, "%d", numToPrint);

                Print(" %-6.6s|", Colors.Cyan, numToPrintStr);

                free(numToPrintStr);
            }
            Print("\n", Colors.Cyan);
        }
    }
}

void DisplayMovieReport(CinemaHallData *halls, int length, MovieDatabase movieData)
{
    for (size_t i = 0; i < movieData->length; i++)
    {
        
    }
    
}

int main()
{
    CinemaHallData hall1 = Cinema.Load(".hall1Save.txt");
    CinemaHallData hall2 = Cinema.Load(".hall2Save.txt");
    CinemaHallData hall3 = Cinema.Load(".hall3Save.txt");
    MovieDatabase movieData = Movie.Load(".movies.txt");

    CinemaHallData *halls = (CinemaHallData *)uMemAlloc(sizeof(struct CinemaHallData) * 3);
    halls[0] = hall1;
    halls[1] = hall2;
    halls[2] = hall3;

    while (1)
    {
        int action = AskMenu("Main Menu : ", "Choose action : ", 5, Colors.Cyan, Colors.Yellow, "Edit hall 1", "Edit hall 2", "Edit hall 3", "Edit Movie Database", "Generate Report");

        switch (action)
        {
        case 1:
            Cinema.EditMovieHallSchedule(hall1, movieData);
            break;
        case 2:
            Cinema.EditMovieHallSchedule(hall2, movieData);
            break;
        case 3:
            Cinema.EditMovieHallSchedule(hall3, movieData);
            break;
        case 4:
            Movie.EditDatabase(movieData);
            break;
        case 5:
            DisplayHallReport(hall1, 1);
            Cinema.DisplayMovieHallSchedule(hall1, movieData);
            DisplayHallReport(hall2, 2);
            Cinema.DisplayMovieHallSchedule(hall2, movieData);
            DisplayHallReport(hall3, 3);
            Cinema.DisplayMovieHallSchedule(hall3, movieData);

            
            DisplayMovieReport(halls, 3, movieData);
            break;
        }

        Cinema.SerializeHallData(".hall1Save.txt", hall1);
        Cinema.SerializeHallData(".hall2Save.txt", hall2);
        Cinema.SerializeHallData(".hall3Save.txt", hall3);
        Movie.SerializeMovieDatabase(".movies.txt", movieData);
    }
}