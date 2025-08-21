#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#include <stdlib.h>
#include <stdio.h>

#include "player.h"

#define SCALE 3

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
    } 
    const Uint8* key = SDL_GetKeyboardState(NULL);
    if (key[SDL_SCANCODE_ESCAPE]) {
        return 0;
    }
    return 1;
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
        WINDOW_W * SCALE,
        WINDOW_H * SCALE,
        0
    );
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, flags);
    SDL_RenderSetScale(rend, (float)SCALE, (float)SCALE);
    SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN);

    Uint32 delta_t = 0;
    Uint32 time_i = 0;
    Uint32 time_f = 0;
    Uint32 game = 1;
    int stage = 0;

    Player *player = CreatePlayer(rend, WINDOW_W/2, WINDOW_H/2 - TILE_SIZE * 2);
    Level *level = CreateLevel(rend, stage);

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
