#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define ANSI_COLOR_RED     "\033[31m"
#define ANSI_COLOR_GREEN   "\033[32m"
#define ANSI_COLOR_YELLOW  "\033[33m"
#define ANSI_COLOR_BLUE    "\033[34m"
#define ANSI_COLOR_MAGENTA "\033[35m"
#define ANSI_COLOR_CYAN    "\033[36m"
#define ANSI_COLOR_RESET   "\033[0m"

//This variable is strictly used for enabling debug printf's
static int isDebugging = 1;

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
static void printDebugText(char text[], ...){
    va_list(args);
    
    char color[] = ANSI_COLOR_YELLOW;
    char colorTerminator[] = ANSI_COLOR_RESET;
    char *newStr = (char*)malloc(100 * sizeof(char));
    strcpy(newStr, "");
    strcat(newStr, color);
    strcat(newStr, text);
    strcat(newStr, colorTerminator);

    va_start(args, text);
    vprintf(newStr, args);
    va_end(args);

    free(newStr);
}
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
    char *labelgap = GetLetterStr(' ', 6 - strlen(label));
    char *leftgap = GetLetterStr(' ', 6 - GetDigits(left));
    char *rightgap = GetLetterStr(' ', 6 - GetDigits(right));
    printf("%s%s : %i%s.%s%i\n", label, labelgap, left, leftgap, rightgap, right);
}
static void Display2ColTitle(char label[], char left[], char right[])
{
    char *labelgap = GetLetterStr(' ', 6 - strlen(label));
    char *leftgap = GetLetterStr(' ', 6 - strlen(left));
    char *rightgap = GetLetterStr(' ', 6 - strlen(right));
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