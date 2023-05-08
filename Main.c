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
    CinemaHallData hall = Cinema.Create();
    MovieDatabase movieData = Movie.Create();

    while (1)
    {
        Print("Main Menu : \n", Colors.Cyan);
        Print("[1] edit hall\n", Colors.Cyan);
        Print("[2] edit movie database\n", Colors.Cyan);
        int action = AskInt("Enter action : ");

        switch (action)
        {
        case 1:
            Cinema.DisplayMovieHallSchedule(hall, movieData);
            break;
        case 2:
            Movie.EditDatabase(movieData);
            break;
        }
    }
}