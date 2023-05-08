typedef struct MovieData
{
    char *title;
    int rating;
    char *summary;
    char *actors;
} *MovieData;

typedef struct MovieDatabase
{
    MovieData *Movies;
    int length;
} *MovieDatabase;

extern const struct Movie{
    void (*EditDatabase)(MovieDatabase database);
    MovieDatabase (*Create)();
} Movie;

typedef struct SeatData{
    char *buyer;
    int price;
} *SeatData;

typedef struct MovieSlotData
{
    char seatingArr;
    int onhold;
    int reserved;
    SeatData *LowerBox;
    SeatData *UpperBox;
    MovieData movie;
} *MovieSlotData;

typedef union CinemaScheduleData
{
    struct dataSlots
    {
        MovieSlotData slot_10;
        MovieSlotData slot_13;
        MovieSlotData slot_16;
    } slot;

    MovieSlotData slots[3];
} *CinemaScheduleData;

typedef struct CinemaHallData
{
    CinemaScheduleData *schedule;
} *CinemaHallData;

extern const struct Cinema
{
    void (*DisplayMovieHallSchedule)(CinemaHallData hall, MovieDatabase database);
    CinemaHallData (*Create)();
} Cinema;