/*

The ORIGINAL CODE is the `NewASM Rendering Engine Window` Source Code.
The INITIAL DEVELOPER is Brace Software Co., DEntisT.
The COPYRIGHT YEAR is 2026.

Version: MPL 1.1

The contents of this file are subject to the Mozilla Public License Version 
1.1 the "License"; you may not use this file except in compliance with 
the License. You may obtain a copy of the License at 
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS" basis,
WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
for the specific language governing rights and limitations under the
License.

Portions created by the Initial Developer are Copyright (c) The COPYRIGHT YEAR
the Initial Developer. All Rights Reserved.

*/

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <stdio.h>

#define SCREEN_COLS 80
#define SCREEN_ROWS 25
#define FONT_WIDTH 10
#define FONT_HEIGHT 18

typedef struct
{
    char ch;
    SDL_Color fg;
    SDL_Color bg;
} Cell;

Cell screen[SCREEN_ROWS][SCREEN_COLS];
int cursorX = 0, cursorY = 0;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
TTF_Font *font = NULL;
int quit = 0;

void openConsoleWindow(const char* title)
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        return;
    }
    if(TTF_Init() == -1)
    {
        return;
    }

    window = SDL_CreateWindow(title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_COLS * FONT_WIDTH, SCREEN_ROWS * FONT_HEIGHT, 0);
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    font = TTF_OpenFont("resources.ttf", FONT_HEIGHT); 
    if (!font) {
        printf("Font error: %s\n", TTF_GetError());
    }
    
    memset(screen, 0, sizeof(screen));
    return;
}

void renderScreen()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int y = 0; y < SCREEN_ROWS; y++)
    {
        for(int x = 0; x < SCREEN_COLS; x++)
        {
            Cell c = screen[y][x];
            SDL_Rect dst = { x * FONT_WIDTH, y * FONT_HEIGHT, FONT_WIDTH, FONT_HEIGHT };

            SDL_SetRenderDrawColor(renderer, c.bg.r, c.bg.g, c.bg.b, 255);
            SDL_RenderFillRect(renderer, &dst);

            if(c.ch > 31)
            {
                char str[2] = { c.ch, 0 };
                SDL_Surface *surf = TTF_RenderText_Solid(font, str, c.fg);
                if(surf)
                {
                    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
                    SDL_Rect charRect = { dst.x, dst.y, surf->w, surf->h };
                    SDL_RenderCopy(renderer, tex, NULL, &charRect);
                    
                    SDL_DestroyTexture(tex);
                    SDL_FreeSurface(surf);
                }
            }
        }
    }
    SDL_RenderPresent(renderer);
    return;
}

void scrollScreen()
{
    memmove(&screen[0], &screen[1], sizeof(Cell) * (SCREEN_ROWS - 1) * SCREEN_COLS);
    memset(&screen[SCREEN_ROWS - 1], 0, sizeof(Cell) * SCREEN_COLS);
    cursorY = SCREEN_ROWS - 1;
    return;
}

void putChar(char ch, SDL_Color fg, SDL_Color bg)
{
    if(ch == '\n')
    {
        cursorX = 0;
        cursorY++;
        if(cursorY >= SCREEN_ROWS)
        {
            scrollScreen();
        }
        return;
    }

    if(cursorX >= SCREEN_COLS)
    {
        cursorX = 0;
        cursorY++;
    }
    if(cursorY >= SCREEN_ROWS)
    {
        scrollScreen();
    }

    screen[cursorY][cursorX].ch = ch;
    screen[cursorY][cursorX].fg = fg;
    screen[cursorY][cursorX].bg = bg;
    cursorX++;
    return;
}

void printTextToConsole(const char* text, SDL_Color fg, SDL_Color bg)
{
    for(int i = 0; text[i]; i++)
    {
        putChar(text[i], fg, bg);
    }
    renderScreen();
}

void requestInputFromConsole(char* buffer, int size, SDL_Color fg, SDL_Color bg)
{
    int pos = 0;
    SDL_StartTextInput();
    int inputDone = 0;

    while(!inputDone && !quit)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
            {
                quit = 1;
                inputDone = 1;
            }
            else if(e.type == SDL_TEXTINPUT)
            {
                if(pos < size - 1)
                {
                    buffer[pos++] = e.text.text[0];
                    buffer[pos] = 0;
                    putChar(e.text.text[0], fg, bg);
                    renderScreen();
                }
            }
            else if(e.type == SDL_KEYDOWN)
            {
                if(e.key.keysym.sym == SDLK_RETURN)
                {
                    inputDone = 1;
                }
                else if(e.key.keysym.sym == SDLK_BACKSPACE && pos > 0)
                {
                    pos--;
                    buffer[pos] = 0;
                    cursorX--;
                    if(cursorX < 0)
                    { 
                        cursorX = SCREEN_COLS - 1; 
                        cursorY--; 
                    }
                    screen[cursorY][cursorX].ch = ' ';
                    renderScreen();
                }
            }
        }
        SDL_Delay(10);
    }
    SDL_StopTextInput();
    putChar('\n', fg, bg);
    renderScreen();
    return;
}

void closeConsoleWindow()
{
    if(font)
    {
        TTF_CloseFont(font);
    }
    if(renderer)
    {
        SDL_DestroyRenderer(renderer);
    }
    if(window)
    {
        SDL_DestroyWindow(window);
    }
    TTF_Quit();
    SDL_Quit();
}

#if 0
int main(int argc, char* argv[])
{
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color black = {0, 0, 0, 255};
    SDL_Color blue = {0, 0, 150, 255};

    openConsoleWindow("SDL VGA Emulator");

    printTextToConsole("Welcome to SDL Screen Emulator\n", white, blue);
    printTextToConsole("Type something: ", white, black);

    char input[128] = {0};
    requestInputFromConsole(input, sizeof(input), white, black);

    if(!quit)
    {
        char reply[256];
        snprintf(reply, sizeof(reply), "You typed: %s\n", input);
        printTextToConsole(reply, white, black);
        
        SDL_Delay(3000);
    }

    closeConsoleWindow();
    return 0;
}
#endif