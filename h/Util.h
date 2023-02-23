#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Numerical
static int GetDigits(int num)
{
    if (num != 0)
    {
        int digits = 0;
        while (num != 0)
        {
            num /= 10;
            digits++;
        }
        return digits;
    }
    else
    {
        return 1;
    }
}

// Text Output Related
static void PrintDivider()
{
    printf("================\n");
}
static char *GetLetterStr(char letter, int length)
{
    char *str = malloc(length + 1);
    memset(str, letter, length);
    str[length] = '\0';
    return str;
}
static void Display2ColValues(char label[], int left, int right)
{
    char *labelgap = GetLetterStr(' ', 5 - sizeof(label));
    char *leftgap = GetLetterStr(' ', 5 - GetDigits(left));
    char *rightgap = GetLetterStr(' ', 5 - GetDigits(right));
    printf("%s%s : %i%s.%s%i\n", label, labelgap, left, leftgap, rightgap, right);
}
static void Display2ColTitle(char label[], char left[], char right[])
{
    char *labelgap = GetLetterStr(' ', 5 - sizeof(label));
    char *leftgap = GetLetterStr(' ', 6 - sizeof(left));
    char *rightgap = GetLetterStr(' ', 6 - sizeof(right));
    printf("%s%s : %s%s.%s%s\n", label, labelgap, left, leftgap, rightgap, right);
}

// Input Related
static float AskFloat(char question[])
{
    float toReturn;
    printf(question);
    scanf("%f", &toReturn);
    return toReturn;
}
static int AskInt(char question[])
{
    printf(question);

    int input, temp, status;

    status = scanf("%d", &input);
    while (status != 1)
    {
        while ((temp = getchar()) != EOF && temp != '\n')
            ;
        printf("[INVALID] Enter a number: ");
        status = scanf("%d", &input);
    }

    return input;
}

// Randomizers
static void SetSeed()
{
    static time_t t;
    static int initializedThisExec;

    if (initializedThisExec != 1)
    {
        initializedThisExec = 1;
        srand((unsigned)time(&t));
    }
}
static int RandomRange(int min, int max)
{
    SetSeed();
    float normalizedRand = (float)rand() / (float)RAND_MAX;
    int range = max - min;
    return (int)((normalizedRand * (float)range) + (float)min);
}
static int CoinToss()
{
    SetSeed();
    float normalizedRand = (float)rand() / (float)RAND_MAX;
    return normalizedRand > 0.5f ? 1 : 0;
}