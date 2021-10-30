#include <iostream>
#include <vector>
#include <thread>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32 | _WIN64
#include <Windows.h>

#else
#include <ncurses.h>

#endif

using namespace std;

void InitArray(vector<vector<int>>& vGrid, int nHeight, int nWidth, bool bRandom = true)
{
    srand(time(NULL));
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

#ifdef _WIN32 | _WIN64
void GetScreenSize(HANDLE hConsole, int& nHeight, int& nWidth)
{
    CONSOLE_SCREEN_BUFFER_INFO *pConsoleInfo = new CONSOLE_SCREEN_BUFFER_INFO;
    GetConsoleScreenBufferInfo(hConsole, pConsoleInfo);
    nWidth = abs(pConsoleInfo->srWindow.Right - pConsoleInfo->srWindow.Left) + 1;
    nHeight = abs(pConsoleInfo->srWindow.Bottom - pConsoleInfo->srWindow.Top) + 1;
}
#else
void GetScreenSize(int& nHeight, int& nWidth) { getmaxyx(stdscr, nHeight, nWidth); }
#endif


int main()
{


    // ========== Set up graphics ========== //
#ifdef _WIN64 | _WIN32
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_WRITE | GENERIC_READ, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole); 
    DWORD dBytesWritten = 0;
#else
    initscr();
    cbreak();
    noecho();
#endif

    // Initialize grid and get screen size
    int nHeight, nWidth;
#ifdef _WIN64 | WIN32
    GetScreenSize(hConsole, nHeight, nWidth);
#else
    GetScreenSize(nHeight, nWidth);
#endif

    vector<vector<int>> vGrid(nHeight, vector<int>(nWidth));

    // ========== Setting up the grid ========== //
    InitArray(vGrid, nHeight, nWidth);
    


    // ========== Game loop ========== //
    while (1)
    {

        // ========== Input ========== //
#ifdef _WIN32 | _WIN64
        if (GetAsyncKeyState(VK_SPACE)) InitArray(vGrid, nHeight, nWidth);
#else
        if (getch() == ' ') InitArray(vGrid, nHeight, nWidth);
#endif

        // ========== Compute ========== //
        // Resize grid according to screen size.
#ifdef _WIN64 | WIN32
        GetScreenSize(hConsole, nHeight, nWidth);
#else
        GetScreenSize(nHeight, nWidth);
#endif
        ResizeGrid(vGrid, nWidth, nHeight);
#ifdef _WIN64 | WIN32
        // Make sure the buffer size is the same as the screen. 
        SetConsoleScreenBufferSize(hConsole, {short(nWidth), short(nHeight)}); 
#endif

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
        wchar_t cAlive;
        wchar_t cDead;
        // cAlive = 0x2588;
        // cDead = 0x2591;
        cAlive = 0x2588;
        cDead = ' ';
#ifdef _WIN32 | _WIN64
        wchar_t *pScreen = new wchar_t[nWidth * nHeight];
        for (int y = 0; y < nHeight; y++)
            for (int x = 0; x < nWidth; x++)
                pScreen[y * nWidth + x] = (vGrid[y][x] == 1) ? cAlive : cDead;
        WriteConsoleOutputCharacterW(hConsole, pScreen, nWidth * nHeight, {0, 0}, &dBytesWritten);
#else
        for (int y = 0; y < nHeight; y++)
            for (int x = 0; x < nWidth; x++)
                mvaddch(y, x, (vGrid[y][x] == 1) ? cAlive : cDead);
#endif

        // ========== Timing ========== //
        this_thread::sleep_for(50ms);
    }
    return 0;
}
