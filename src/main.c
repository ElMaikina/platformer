#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#include <stdlib.h>
#include <stdio.h>

#include "player.h"

void ClearScreen(SDL_Renderer* rend) {
    SDL_RenderClear(rend);
}

void DrawScreen(SDL_Renderer* rend) {
    SDL_RenderPresent(rend);
}

Uint32 CheckGameState() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return 0;
        }
    } return 1;
}

int main(int argc, char *argv[]) {
    Uint32 flags = SDL_RENDERER_ACCELERATED;
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("Error initializing SDL: %s", SDL_GetError());
    }
    SDL_Window* win = SDL_CreateWindow(
        "Platformer in C",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0
    );
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, flags);
	//SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN);
	//SDL_RenderSetIntegerScale(rend, SDL_TRUE);
    Uint32 delta_t = 0;
    Uint32 time_i = 0;
    Uint32 time_f = 0;
    Uint32 game = 1;
    
    Player *player = CreatePlayer(rend, WINDOW_WIDTH/2 + 800, WINDOW_HEIGHT/2 - TILE_SIZE * 2);
    Level *level = CreateLevel(rend, 100, 35);

    for (Uint32 i = 0; i < level->w; ++i) {
        AddTileToLevel(level, i, 26);
    }

    AddTileToLevel(level, 8, 25);
    AddTileToLevel(level, 8, 24);
    AddTileToLevel(level, 8, 23);
    AddTileToLevel(level, 8, 22);
    AddTileToLevel(level, 8, 21);

    AddTileToLevel(level, 30, 25);
    AddTileToLevel(level, 30, 24);
    AddTileToLevel(level, 30, 23);

    AddTileToLevel(level, 44, 23);
    AddTileToLevel(level, 45, 23);
    AddTileToLevel(level, 46, 23);
    AddTileToLevel(level, 47, 23);
    AddTileToLevel(level, 48, 23);

    AddTileToLevel(level, 44, 23-5);
    AddTileToLevel(level, 45, 23-5);
    AddTileToLevel(level, 46, 23-5);
    AddTileToLevel(level, 47, 23-5);
    AddTileToLevel(level, 48, 23-5);

    AddTileToLevel(level, 44-10, 23-7);
    AddTileToLevel(level, 45-10, 23-7);
    AddTileToLevel(level, 46-10, 23-7);
    AddTileToLevel(level, 47-10, 23-7);
    AddTileToLevel(level, 48-10, 23-7);

    AddTileToLevel(level, 44+10, 20);
    AddTileToLevel(level, 45+10, 20);
    AddTileToLevel(level, 46+10, 20);
    AddTileToLevel(level, 47+10, 20);
    AddTileToLevel(level, 48+10, 20);

    AddTileToLevel(level, 44+14, 16);
    AddTileToLevel(level, 45+14, 16);
    AddTileToLevel(level, 46+14, 16);
    AddTileToLevel(level, 47+14, 16);
    AddTileToLevel(level, 48+14, 16);

    AddTileToLevel(level, 48+15, 12);
    AddTileToLevel(level, 48+15, 11);
    AddTileToLevel(level, 48+15, 10);
    AddTileToLevel(level, 48+15, 9);
    AddTileToLevel(level, 48+15, 8);
    AddTileToLevel(level, 48+15, 7);

    AddTileToLevel(level, 48+10, 12-3);
    AddTileToLevel(level, 48+10, 11-3);
    AddTileToLevel(level, 48+10, 10-3);
    AddTileToLevel(level, 48+10, 9-3);
    AddTileToLevel(level, 48+10, 8-3);
    AddTileToLevel(level, 48+10, 7-3);
    
    while (game) {
        game = CheckGameState();
        time_f = SDL_GetTicks();
        delta_t = time_f - time_i;
        if (delta_t > 1000 / WINDOW_RATE) {
            ClearScreen(rend);
            MovePlayer(player, level);
            MoveCamera(player, level, rend);
            DrawLevel(player, level, rend);
            DrawPlayer(player, level, rend);
            DrawScreen(rend);
            time_i = time_f;
        }
    }

    FreePlayer(player);
    FreeLevel(level);

    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
