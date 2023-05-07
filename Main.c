/*
NOTE: When compiling, make sure to include ALL THREE *.c files in the compilation inputs.
    This is so that the linker will be able to link prototypes from the *.h files to their respective definitions in the *.c files.

FILES NEEDED TO INCLUDE:
    Main.c
    Entity.c
    u_list.c
*/

#include "h/u_stdio.h"

int main()
{
    CinemaHall hall = Cinema.Create();

    while (1)
    {
        Print("1: GetState\n", Colors.Cyan);
        Print("2: DisplaySeats\n", Colors.Cyan);
        Print("3: DisplaySchedule\n", Colors.Cyan);
        Print("4: Modify\n", Colors.Cyan);
        int choice = AskInt("Enter Command : ");

        switch (choice)
        {
        case 1:
            Cinema.GetState(hall);
            break;
        case 3:
            Cinema.DisplaySchedule(hall);
            break;
        case 4:
            Cinema.Modify(hall);
            break;
        default:
            break;
        }
    }
}