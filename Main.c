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
        Cinema.DisplaySchedule(hall);
    }
}