#include <iostream>
#include <vector>
#include <thread>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>

using namespace std;

void InitArray(vector<vector<int>>& vGrid, int nHeight, int nWidth, bool bRandom = true)
{
    vGrid = vector<vector<int>>(nHeight,vector<int>(nWidth,0));
    int nMargin = 4;
    if (bRandom)
    {
        for (int y = nHeight / 2 - nMargin; y < nHeight / 2 + nMargin; y++)
            for (int x = nWidth / 2 - nMargin; x < nWidth / 2 + nMargin; x++)
                vGrid[y][x] = rand()%2;
    }
}

void ResizeGrid(vector<vector<int>>& vGrid, int nWidth, int nHeight)
{
    for (vector<int>& i : vGrid)
    {
        i.resize(nWidth, 0);
    }
    vGrid.resize(nHeight, vector<int>(nWidth, 0));
}


int main()
{


    // ========== Set up graphics ========== //
    setlocale(LC_ALL, "");
    initscr();
    nodelay(stdscr, true);
    noecho();
    keypad(stdscr, true);

    // Initialize grid and get screen size
    int nWidth = getmaxx(stdscr);
    int nHeight = getmaxy(stdscr);
    vector<vector<int>> vGrid(nHeight, vector<int>(nWidth));
    srand(time(NULL));
    InitArray(vGrid, nHeight, nWidth);
    

    // ========== Setting up the grid ========== //

    // ========== Game loop ========== //
    while (1)
    {

        // ========== Input ========== //
        int nInput = getch();
        if (nInput == ' ')
            InitArray(vGrid, nHeight, nWidth);
        if (nInput == 27)
            break;

        // ========== Compute ========== //
        // Resize grid according to screen size.
        getmaxyx(stdscr,nHeight, nWidth);
        ResizeGrid(vGrid, nWidth, nHeight);

        // The Game of Life!
        vector<vector<int>> vNext(nHeight, vector<int>(nWidth,0));
        for (int y = 0; y < nHeight; y++)
        {
            for (int x = 0; x < nWidth; x++)
            {
                // Count the neighbours
                int nNeighbours = 0;
                for (int i : {x - 1, x, x + 1})
                    for (int j : {y - 1, y, y + 1})
                    {
                        if (i == nWidth || i == -1)
                            i = nWidth - abs(i);
                        if (j == nHeight || j == -1)
                            j = nHeight - abs(j);
                        nNeighbours += vGrid[j][i];
                    }
                nNeighbours -= vGrid[y][x];
                // Apply the rules
                int nState;
                if (vGrid[y][x] == 1)
                    nState = (nNeighbours == 2 || nNeighbours == 3)? 1 : 0;
                else
                    nState = nNeighbours == 3? 1 : 0;
                // Setting new state to next grid
                vNext[y][x] = nState;
            }
        }
        vGrid = vNext;

        // ========== Render ========== //
        char cAlive;
        char cDead;
        // cAlive = 0x2588;
        // cDead = 0x2591;
        cAlive = '\u2588';
        cDead = ' ';
        for (int y = 0; y < nHeight; y++)
        {
            for (int x = 0; x < nWidth; x++)
            {
                mvaddch(y, x, (vGrid[y][x] == 1? cAlive : cDead));
            }
        }
        refresh();

        // ========== Timing ========== //
        this_thread::sleep_for(50ms);
    }
    endwin();
    return 0;
}
