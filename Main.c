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
    CinemaHallData hall1 = Cinema.Load(".hall1Save.txt");
    CinemaHallData hall2 = Cinema.Load(".hall2Save.txt");
    CinemaHallData hall3 = Cinema.Load(".hall3Save.txt");
    MovieDatabase movieData = Movie.Load(".movies.txt");

    while (1)
    {
        int action = AskMenu("Main Menu : ", "Choose action : ", 4, Colors.Cyan, Colors.Yellow, "Edit hall 1", "Edit hall 2", "Edit hall 3", "edit movie database");

        switch (action)
        {
        case 1:
            Cinema.DisplayMovieHallSchedule(hall1, movieData);
            break;
        case 2:
            Cinema.DisplayMovieHallSchedule(hall2, movieData);
            break;
        case 3:
            Cinema.DisplayMovieHallSchedule(hall3, movieData);
            break;
        case 4:
            Movie.EditDatabase(movieData);
            break;
        }

        Cinema.SerializeHallData(".hall1Save.txt", hall1);
        Cinema.SerializeHallData(".hall2Save.txt", hall2);
        Cinema.SerializeHallData(".hall3Save.txt", hall3);
        Movie.SerializeMovieDatabase(".movies.txt", movieData);
    }
}