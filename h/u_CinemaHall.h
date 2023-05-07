typedef struct MovieData
{
    char *title;
    int rating;
    char *summary;
    char **actors;
} *MovieData;

typedef struct MovieSlotData
{
    int onhold;
    int reserved;
    char *LowerBox;
    char *UpperBox;
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

typedef struct CinemaHall
{
    char seatingArr;
    CinemaScheduleData *schedule;
} *CinemaHall;

extern const struct Cinema
{
    char *(*GetState)(MovieSlotData slot);
    void (*DisplaySchedule)(CinemaHall hall);
    void (*Modify)(CinemaHall hall);
    CinemaHall (*Create)();
} Cinema;