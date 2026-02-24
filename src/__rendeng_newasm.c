// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

// EXPERIMENTAL FILE!
// May be added in the future
// Currently SDL is NOT included in the final binaries....

/*
why don't we have std::media::window, std::media::renderer, etc. xD
*/

extern "C"
{
    #define SDL_MAIN_HANDLED
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_ttf.h>
    #include <string.h>
    #include <stdio.h>
    #include <stdbool.h>

    #define SCREEN_COLS 80
    #define SCREEN_ROWS 25
    #define FONT_WIDTH 10
    #define FONT_HEIGHT 18

    typedef struct
    {
        char ch;
        SDL_Color fg;
        SDL_Color bg;
        bool dirty; 
    } Cell;

    Cell screen[SCREEN_ROWS][SCREEN_COLS];
    int cursorX = 0, cursorY = 0;

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    TTF_Font *font = NULL;
    int quit = 0;

    SDL_Color white     = {255, 255, 255, 255};
    SDL_Color black     = {0, 0, 0, 255};
    SDL_Color red       = {255, 0, 0, 255};
    SDL_Color yellow    = {255, 255, 0, 255};
    SDL_Color green     = {0, 255, 0, 255};
    SDL_Color blue      = {0, 0, 255, 255};
    SDL_Color magenta   = {255, 0, 255, 255};
    SDL_Color cyan      = {0, 255, 255, 255};
    SDL_Color gray      = {128, 128, 128, 255};

    SDL_Color current = white;

    SDL_Texture* glyphCache[256];

    extern void openConsoleWindow(const char* title)
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
        
        font = TTF_OpenFont("nfont.ttf", FONT_HEIGHT); 
        if(!font)
        {
            printf("Font error: %s\n", TTF_GetError());
            return;
        }

        for(int y = 0; y < SCREEN_ROWS; y++)
        {
            for(int x = 0; x < SCREEN_COLS; x++)
            {
                screen[y][x].ch = ' ';
                screen[y][x].fg = white;
                screen[y][x].bg = black;
                screen[y][x].dirty = true;
            }
        }

        for(int i = 0; i < 256; i++)
        {
            char str[2] = {
                (char)i,
                0
            };
            SDL_Surface* surf = TTF_RenderText_Solid(font, str, white);
            if(surf)
            {
                glyphCache[i] = SDL_CreateTextureFromSurface(renderer, surf);
                SDL_FreeSurface(surf);
            }
            else
            {
                glyphCache[i] = NULL;
            }
        }
    }

    extern void renderScreen()
    {
        for (int y = 0; y < SCREEN_ROWS; y++)
        {
            for(int x = 0; x < SCREEN_COLS; x++)
            {
                Cell *c = &screen[y][x];
                
                if(!c->dirty) continue;

                SDL_Rect dst = {
                    x * FONT_WIDTH,
                    y * FONT_HEIGHT,
                    FONT_WIDTH,
                    FONT_HEIGHT
                };

                SDL_SetRenderDrawColor(renderer, c->bg.r, c->bg.g, c->bg.b, 255);
                SDL_RenderFillRect(renderer, &dst);

                if(c->ch > 31 && glyphCache[(unsigned char)c->ch])
                {
                    SDL_Texture* tex = glyphCache[(unsigned char)c->ch];
                    SDL_SetTextureColorMod(tex, c->fg.r, c->fg.g, c->fg.b);
                    SDL_RenderCopy(renderer, tex, NULL, &dst);
                }

                c->dirty = false;
            }
        }
        SDL_RenderPresent(renderer);
    }

    extern void scrollScreen()
    {
        memmove(&screen[0], &screen[1], sizeof(Cell) * (SCREEN_ROWS - 1) * SCREEN_COLS);
        
        for(int x = 0; x < SCREEN_COLS; x++)
        {
            screen[SCREEN_ROWS-1][x].ch = ' ';
            screen[SCREEN_ROWS-1][x].fg = white;
            screen[SCREEN_ROWS-1][x].bg = black;
        }

        for(int y = 0; y < SCREEN_ROWS; y++)
        {
            for(int x = 0; x < SCREEN_COLS; x++)
            {
                screen[y][x].dirty = true;
            }
        }

        cursorY = SCREEN_ROWS - 1;
    }

    extern void putChar(char ch, SDL_Color fg, SDL_Color bg)
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

        Cell *c = &screen[cursorY][cursorX];
        c->ch = ch;
        c->fg = fg;
        c->bg = bg;
        c->dirty = true;
        cursorX++;
    }

    extern void printTextToConsole(const char* text, SDL_Color fg, SDL_Color bg)
    {
        for(int i = 0; text[i]; i++)
        {
            putChar(text[i], fg, bg);
        }

        renderScreen();
    }

    extern void setCol(int colid)
    {
        if(colid == 1) current = red;
        if(colid == 2) current = yellow;
        if(colid == 3) current = green;
        if(colid == 4) current = blue;
        if(colid == 5) current = magenta;
        if(colid == 6) current = cyan;
        if(colid == 7) current = gray;
        if(colid == 8) current = white;
    }

    extern void printToConsole(const char* text)
    {
        printTextToConsole(text, current, black);
        return;
    }

    extern void requestInputFromConsole(char* buffer, int size, SDL_Color fg, SDL_Color bg)
    {
        int pos = 0;
        buffer[0] = 0;
        SDL_StartTextInput();
        int inputDone = 0;

        while(!inputDone && !quit)
        {
            SDL_Event e;
            while(SDL_PollEvent(&e))
            {
                if(e.type == SDL_QUIT) { quit = 1; inputDone = 1; }
                else if(e.type == SDL_TEXTINPUT)
                {
                    if(pos < size - 1)
                    {
                        char ch = e.text.text[0];
                        buffer[pos++] = ch;
                        buffer[pos] = 0;
                        putChar(ch, fg, bg);
                        renderScreen();
                    }
                }
                else if(e.type == SDL_KEYDOWN)
                {
                    if(e.key.keysym.sym == SDLK_RETURN) inputDone = 1;
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
                        screen[cursorY][cursorX].dirty = true;
                        renderScreen();
                    }
                }
            }
            SDL_Delay(10);
        }
        SDL_StopTextInput();
        putChar('\n', fg, bg);
        renderScreen();
    }

    extern void inputFromConsole(char* buffer, int size)
    {
        requestInputFromConsole(buffer, size, white, black);
    }


    extern void closeConsoleWindow()
    {
        for(int i = 0; i < 256; i++)
        {
            if(glyphCache[i]) SDL_DestroyTexture(glyphCache[i]);
        }
        if(font) TTF_CloseFont(font);
        if(renderer) SDL_DestroyRenderer(renderer);
        if(window) SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
    }

    extern void cls()
    {
        for (int y = 0; y < SCREEN_ROWS; y++)
        {
            for (int x = 0; x < SCREEN_COLS; x++)
            {
                screen[y][x].ch = ' ';
                screen[y][x].fg = white;
                screen[y][x].bg = black;
                screen[y][x].dirty = true;
            }
        }
        
        cursorX = 0;
        cursorY = 0;

        renderScreen();
    }
}