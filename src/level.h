#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#include <stdlib.h>
#include <stdio.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 360
#define WINDOW_RATE 60
#define TILE_SIZE 12

typedef struct {
    SDL_Surface* surf;
    SDL_Texture* text;
    SDL_Rect rect;
    Uint32 *tiles;
    Uint32 w, h;
} Level;

Level *CreateLevel(SDL_Renderer* rend, Uint32 w, Uint32 h);
Uint32 GetTileFromLevel(Level *l, int x, int y);
void AddTileToLevel(Level *l, int x, int y);
void FreeLevel(Level *l);