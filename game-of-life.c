#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include "console.c"

#define XSIZE 100 // max 236
#define YSIZE 30  // max 61(67)
#define SLEEP 60
#define CHAR 219

void print(int array[XSIZE][YSIZE]); // unused
void qprint(int array[XSIZE][YSIZE]);
void getrandarray(int array[XSIZE][YSIZE]);
int countNachbar(int x, int y, int array[XSIZE][YSIZE]);
void clone(int array1[XSIZE][YSIZE], int array2[XSIZE][YSIZE]); // unused
void calccells(int thisarray[XSIZE][YSIZE], int nextarray[XSIZE][YSIZE]);

int choice;

int main()
{
    setCursorType(0);
    srand(time(0));
    SMALL_RECT windowSize = {0, 0, XSIZE, YSIZE + 4};
    SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &windowSize);
    int cells[XSIZE][YSIZE] = {}, nextcells[XSIZE][YSIZE] = {};
    int cycles = 0;
    //    char c;
    double timetaken, cpuTime, biggestCpuTime = 0;
    printf("Nachbarmodus?\n1 = Rechteck(Randzellen bleiben immer gleich, stirbt nicht oft aus)\n0 = Torus(Die seiten des rechteckes werden verbunden, um einen Donut\ndarzustellen, der keine raender hat, stirbt oefter aus, ist der Originale Modus)\n");
    scanf("%d", &choice);
    clrscr();
    gotoxy(0, 0);
    getrandarray(cells);
    while (1)
    {
        clock_t tic = clock();
        // measure time for 1 cycle
        qprint(nextcells);
        calccells(cells, nextcells);
        cycles++;
        memcpy(cells, nextcells, YSIZE * XSIZE * sizeof(int));
        // if(kbhit())
        //     c = getch();
        delay(SLEEP);
        // Calculate Stats
        clock_t toc = clock();
        timetaken = (double)(toc - tic) / CLOCKS_PER_SEC;
        cpuTime = timetaken - (SLEEP / 1000.0);
        if (cpuTime > biggestCpuTime)
            biggestCpuTime = cpuTime;
        gotoxy(0, YSIZE + 1);
        printf("Time per Cycle: %f s\nDelay:          %f s\nCPU Time:       %f s\nworst CPU Time: %f s\nCycle:          %d", timetaken, SLEEP / 1000.0, cpuTime, biggestCpuTime, cycles);
    }
    return 0;
}

void qprint(int array[XSIZE][YSIZE])
{
    char text[XSIZE * YSIZE + (YSIZE)];
    char at[2];
    *at = CHAR;
    char space[2] = " ";
    char newline[2] = "\n";
    text[0] = '\0';
    for (int j = 0; j < YSIZE; j++)
    {
        for (int i = 0; i < XSIZE; i++)
        {
            if (array[i][j] == 1)
                strcat(text, at);
            else
                strcat(text, space);
        }
        strcat(text, newline);
    }
    gotoxy(0, 0);
    printf("%s", text);
}

void print(int array[XSIZE][YSIZE])
{
    gotoxy(0, 0);
    int x, y;
    for (y = 0; y < YSIZE; y++)
    {
        for (x = 0; x < XSIZE; x++)
        {
            if (array[x][y] == 1)
                printf("%c", CHAR);
            else
                printf(" ");
        }
        printf("\n");
    }
}

void getrandarray(int array[XSIZE][YSIZE])
{
    int x, y;
    for (x = 0; x < XSIZE; x++)
    {
        for (y = 0; y < YSIZE; y++)
        {
            array[x][y] = rand() % 2;
            //array[x][y] = 1;
        }
    }
}

int countNachbar(int x, int y, int array[XSIZE][YSIZE])
{
    int sum = 0, i, j, spalte, reihe;
    for (i = -1; i < 2; i++)
    {
        for (j = -1; j < 2; j++)
        {
            spalte = (x + i + XSIZE) % XSIZE;
            reihe = (y + j + YSIZE) % YSIZE;
            sum += array[spalte][reihe];
        }
    }
    sum -= array[x][y];
    return sum;
}

void clone(int array1[XSIZE][YSIZE], int array2[XSIZE][YSIZE])
{
    int x, y;
    for (x = 0; x < XSIZE; x++)
    {
        for (y = 0; y < YSIZE; y++)
        {
            array2[x][y] = array1[x][y];
        }
    }
}

void calccells(int thisarray[XSIZE][YSIZE], int nextarray[XSIZE][YSIZE])
{
    int x, y, state, nachbarn;
    for (x = 0; x < XSIZE; x++)
    {
        for (y = 0; y < YSIZE; y++)
        {
            state = thisarray[x][y];
            nachbarn = countNachbar(x, y, thisarray);
            if (state == 0 && nachbarn == 3)
            {
                nextarray[x][y] = 1;
            }
            if (state == 1 && (nachbarn < 2 || nachbarn > 3))
                nextarray[x][y] = 0;
            if (choice)
            {
                if (x == 0 || x == (XSIZE - 1) || y == 0 || y == (YSIZE - 1))
                    nextarray[x][y] = thisarray[x][y];
            }
        }
    }
}
