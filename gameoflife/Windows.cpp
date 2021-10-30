#include <iostream>
#include <vector>
#include <thread>
#include <stdlib.h>
#include <Windows.h>
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

int GetScreenSizeX(HANDLE hConsole)
{
    CONSOLE_SCREEN_BUFFER_INFO *pConsoleInfo = new CONSOLE_SCREEN_BUFFER_INFO;
    GetConsoleScreenBufferInfo(hConsole, pConsoleInfo);
    return abs(pConsoleInfo->srWindow.Right - pConsoleInfo->srWindow.Left) + 1;
}

int GetScreenSizeY(HANDLE hConsole)
{
    CONSOLE_SCREEN_BUFFER_INFO *pConsoleInfo = new CONSOLE_SCREEN_BUFFER_INFO;
    GetConsoleScreenBufferInfo(hConsole, pConsoleInfo);
    return abs(pConsoleInfo->srWindow.Bottom - pConsoleInfo->srWindow.Top) + 1;
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
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_WRITE | GENERIC_READ, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole); 
    DWORD dBytesWritten = 0;

    // Initialize grid and get screen size
    int nWidth = GetScreenSizeX(hConsole);
    int nHeight = GetScreenSizeY(hConsole);
    vector<vector<int>> vGrid(nHeight, vector<int>(nWidth));
    srand(time(NULL));
    InitArray(vGrid, nHeight, nWidth);
    

    // ========== Setting up the grid ========== //

    // ========== Game loop ========== //
    while (1)
    {

        // ========== Input ========== //
        if (GetAsyncKeyState(VK_SPACE))
            InitArray(vGrid, nHeight, nWidth);

        // ========== Compute ========== //
        // Resize grid according to screen size.
        nWidth = GetScreenSizeX(hConsole);
        nHeight = GetScreenSizeY(hConsole);
        ResizeGrid(vGrid, nWidth, nHeight);
        // Make sure the buffer size is the same as the screen. 
        SetConsoleScreenBufferSize(hConsole, {short(nWidth), short(nHeight)}); 

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
        wchar_t *pScreen = new wchar_t[nWidth * nHeight];
        wchar_t cAlive;
        wchar_t cDead;
        // cAlive = 0x2588;
        // cDead = 0x2591;
        cAlive = 0x2588;
        cDead = ' ';
        for (int y = 0; y < nHeight; y++)
        {
            for (int x = 0; x < nWidth; x++)
            {
                pScreen[y * nWidth + x] = (vGrid[y][x] == 1) ? cAlive : cDead;
            }
        }
        WriteConsoleOutputCharacterW(hConsole, pScreen, nWidth * nHeight, {0, 0}, &dBytesWritten);

        // ========== Timing ========== //
        this_thread::sleep_for(50ms);
    }
    return 0;
}
