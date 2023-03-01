#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include "u_list.h"
#include "u_mem.h"
#include "Entity.h"

//For enabling debug prints (mostly memory address allocations)
#define USE_DEBUGPRINTS 0
//For enabling color escape characters (in case not compatible)
#define USE_COLOREDTEXT 1

//Color is simply an escape character, however, for less confusion, a custon type is defined for it.
typedef char *Color;
//Used for more readable displays and lessens the need for dividers and headers.
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
// Print Related
//==============================

//Returns a line of string, which is made up of a specific letter and of specified length.
static char *GetLetterStr(char letter, int length)
{
    char *str = (char*)uMemAlloc(length + 1);
    memset(str, letter, length);
    str[length] = '\0';
    return str;
}

//Gets the digits of an int using iterative integer division by ten.
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

/*
A user defined wrapper for the vprintf function.
This function is also meant to be wrapped and abstracted by other custom print functions.
This is meant to give an extra Color functionality to the printf function which suits the program's needs.
*/
static void vPrint(char text[], Color color, va_list args)
{
    char *newStr = (char *)uMemAlloc(100 * sizeof(char));
    strcpy(newStr, "");
    if (USE_COLOREDTEXT)
        strcat(newStr, color);
    strcat(newStr, text);
    if (USE_COLOREDTEXT)
        strcat(newStr, Colors.Reset);

    vprintf(newStr, args);

    free(newStr);
}

//A wrapper for the vPrint function which abstracts the va_list parameter to the standard multiple arg passing.
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

//A wrapper for the vPrint function which only runs if the USE_DEBUGPRINTS macro is true/1.
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

//A wrapper for a print function which prints a red divider. Made for consistency.
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
    //Freeing allocated memory on use.
    free(labelgap);
    free(leftgap);
    free(rightgap);
}
static void Display2ColTitle(char label[], char left[], char right[])
{
    char *labelgap = GetLetterStr(' ', 6 - strlen(label));
    char *leftgap = GetLetterStr(' ', 6 - strlen(left));
    char *rightgap = GetLetterStr(' ', 6 - strlen(right));
    Print("%s%s : %s%s.%s%s\n", Colors.Green, label, labelgap, left, leftgap, rightgap, right);
    //Freeing allocated memory on use.
    free(labelgap);
    free(leftgap);
    free(rightgap);
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

//==============================
// Math
//==============================

static int Max(int val1, int val2){
    return val1 < val2 ? val2 : val1;
}

static int Min(int val1, int val2){
    return val1 > val2 ? val2 : val1;
}