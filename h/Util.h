#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

// This variable is strictly used for enabling debug printf's
#define USE_DEBUGPRINTS 1
#define USE_COLOREDTEXT 1

typedef char *Color;
static const struct Colors
{
    const Color Red;
    const Color Green;
    const Color Yellow;
    const Color Blue;
    const Color Magenta;
    const Color Cyan;
    const Color Reset;
} Colors = {
    .Red = "\033[31m",
    .Green = "\033[32m",
    .Yellow = "\033[33m",
    .Blue = "\033[34m",
    .Magenta = "\033[35m",
    .Cyan = "\033[36m",
    .Reset = "\033[0m"};

//==============================
// Prints
//==============================

static char *GetLetterStr(char letter, int length)
{
    char *str = malloc(length + 1);
    memset(str, letter, length);
    str[length] = '\0';
    return str;
}

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
static void vPrint(char text[], Color color, va_list args)
{
    char *newStr = (char *)malloc(100 * sizeof(char));
    strcpy(newStr, "");
    if (USE_COLOREDTEXT)
        strcat(newStr, color);
    strcat(newStr, text);
    if (USE_COLOREDTEXT)
        strcat(newStr, Colors.Reset);

    vprintf(newStr, args);

    free(newStr);
}
static void Print(char text[], Color color, ...)
{
    va_list(args);

    va_start(args, color);
    vPrint(text, color, args);
    va_end(args);
}
// ##############
//  ^^^ Dependencies
// ##############
static void printDebugText(char text[], ...)
{
    if (USE_DEBUGPRINTS)
    {
        va_list(args);

        va_start(args, text);
        vPrint(text, Colors.Yellow, args);
        va_end(args);
    }
}
static void PrintDivider()
{
    Print("================\n", Colors.Red);
}
static void Display2ColValues(char label[], int left, int right)
{
    char *labelgap = GetLetterStr(' ', 6 - strlen(label));
    char *leftgap = GetLetterStr(' ', 6 - GetDigits(left));
    char *rightgap = GetLetterStr(' ', 6 - GetDigits(right));
    Print("%s%s : %i%s.%s%i\n", Colors.Reset, label, labelgap, left, leftgap, rightgap, right);
}
static void Display2ColTitle(char label[], char left[], char right[])
{
    char *labelgap = GetLetterStr(' ', 6 - strlen(label));
    char *leftgap = GetLetterStr(' ', 6 - strlen(left));
    char *rightgap = GetLetterStr(' ', 6 - strlen(right));
    Print("%s%s : %s%s.%s%s\n", Colors.Green, label, labelgap, left, leftgap, rightgap, right);
}

//==============================
// Input
//==============================
static float AskFloat(char question[])
{
    float toReturn;
    Print(question, Colors.Reset);
    scanf("%f", &toReturn);
    return toReturn;
}
static int AskInt(char question[])
{
    Print(question, Colors.Reset);

    int input, temp, status;

    status = scanf("%d", &input);
    while (status != 1)
    {
        while ((temp = getchar()) != EOF && temp != '\n')
            ;
        Print("[INVALID] Enter a number: ", Colors.Red);
        status = scanf("%d", &input);
    }

    return input;
}

//==============================
// Randomizers
//==============================
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
// ##############
//  ^^^ Dependencies
// ##############
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