#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdbool.h>

#include "../include/block.h"
#include "../include/config.h"

Block *createBlock(SDL_Renderer* rend, int x, int y, char sprite[]) {
    // creates the block pointer and allocates memory
    Block *block;
    block = (Block*)malloc(sizeof(Block));

    // creates the surface to be drawn
    SDL_Surface* surface;
    surface = IMG_Load(sprite);

    // creates a texture to use hardware rendering
    SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surface);

    // clears the surface
    SDL_FreeSurface(surface);

    // creates destiny geometry
    SDL_Rect dest;
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    
    // centers the geometry to the center of the screen
    dest.x = x;
    dest.y = y;

    // assigns block variables
    block->surface = surface;
    block->tex = tex;
    block->dest = dest;
    return block;
}