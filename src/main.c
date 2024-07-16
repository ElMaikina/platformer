#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdbool.h>

#include "../include/player.h"
#include "../include/block.h"
#include "../include/physics.h"
#include "../include/config.h"

int main(int argc, char *argv[])
{
    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    // creates a window
    SDL_Window* win = SDL_CreateWindow("Platform Game",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       WINDOW_WIDTH, 
                                       WINDOW_HEIGHT, 
                                       0);
    // set the window to fullscreen
    // SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN);

    // triggers the program that controls
    // your graphics hardware and sets flags
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
 
    // creates a renderer to render our images
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);

    // create the main player
    Player *player = createPlayer(rend, "./image/round.png");

    // stores all the bullets to be drawn in a linked list
    Block *blocks[] = {
        createBlock(rend, 22 * 12, 27 * 12, "./image/block.png"),
        createBlock(rend, 23 * 12, 27 * 12, "./image/block.png"),
        createBlock(rend, 24 * 12, 27 * 12, "./image/block.png"),
        createBlock(rend, 29 * 12, 25 * 12, "./image/block.png"),
        createBlock(rend, 30 * 12, 25 * 12, "./image/block.png"),
        createBlock(rend, 31 * 12, 25 * 12, "./image/block.png"),
    };
    int block_len = sizeof(blocks) / sizeof(Block*);
    
    // controls animation loop
    int close = 0;
 
    // animation loop
    while (!close) {

        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                close = 1;
                return 0;
            }
        }
        // clears the screen
        SDL_RenderClear(rend);
        
        // manages player movement
        moveAndControlPlayer(player);

        // apply gravity and physics to player
        applyGravityToPlayer(player);

        // apply limits to player movement
        applyBoundsToPlayer(player);

        // draws all the blocks
        for (int b = 0; b < block_len; b++) {
            SDL_RenderCopy(rend, blocks[b]->tex, NULL, &blocks[b]->dest);
            //player->dest.x = playerCollideHor(player->dest, blocks[b]->dest);
            player->dest.y = playerCollideVer(player->dest, blocks[b]->dest);
        }

        // then draws the player over
        SDL_RenderCopy(rend, player->tex, NULL, &player->dest);
 
        // triggers the double buffers
        // for multiple rendering
        SDL_RenderPresent(rend);
 
        // limits the refresh rate
        SDL_Delay(1000 / WINDOW_FRAMERATE);
    }
 
    // destroy texture
    SDL_DestroyTexture(player->tex);
 
    // destroy renderer
    SDL_DestroyRenderer(rend);
 
    // destroy window
    SDL_DestroyWindow(win);
     
    // close SDL
    SDL_Quit();
 
    return 0;
}
