#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdbool.h>

#include "../include/player.h"

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
    Player *player = createPlayer(rend, "image/round.png");

    // stores all the bullets to be drawn in a linked list
    Block *blocks[] = {
        createBlock(rend, 8 * 12, 23 * 12, "image/block.png"),
        createBlock(rend, 9 * 12, 23 * 12, "image/block.png"),
        createBlock(rend, 10 * 12, 23 * 12, "image/block.png"),
        createBlock(rend, 18 * 12, 23 * 12, "image/block.png"),
        createBlock(rend, 19 * 12, 23 * 12, "image/block.png"),
        createBlock(rend, 20 * 12, 23 * 12, "image/block.png"),
        createBlock(rend, 22 * 12, 27 * 12, "image/block.png"),
        createBlock(rend, 23 * 12, 27 * 12, "image/block.png"),
        createBlock(rend, 24 * 12, 27 * 12, "image/block.png"),
        createBlock(rend, 29 * 12, 25 * 12, "image/block.png"),
        createBlock(rend, 30 * 12, 25 * 12, "image/block.png"),
        createBlock(rend, 31 * 12, 25 * 12, "image/block.png"),
        createBlock(rend, 41 * 12, 360 - 12, "image/block.png"),
        createBlock(rend, 41 * 12, 360 - 24, "image/block.png"),
        createBlock(rend, 41 * 12, 360 - 36, "image/block.png"),
        createBlock(rend, 41 * 12, 360 - 48, "image/block.png"),
    };

    // get quantity of blocks for the render loop
    int block_len = sizeof(blocks) / sizeof(Block*);
    
    // controls animation loop
    int close = 0;

    // count game ticks twice to update according to refresh rate
    Uint32 ticks_a = 0;
    Uint32 ticks_b = 0;

    // get the difference of ticks between a frame and another
    Uint32 delta = 0;
 
    // animation loop
    while (!close) {

        // fetch game ticks
        ticks_a = SDL_GetTicks();
        
        // get delta ticks to limit framerate
        delta = ticks_a - ticks_b;

        // poll os events
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                close = 1;
                return 0;
            }
        }
        
        // limit framerate by adding a delay
        if (delta > 1000 / WINDOW_FRAMERATE) 
        {
            // print fps in terminal
            printf("FPS: %d\n", (1000 / delta));

            // update the tick count
            ticks_b = ticks_a;

            // clears the screen
            SDL_RenderClear(rend);
            
            // manages player movement
            moveAndControlPlayer(player);

            // apply gravity and physics to player
            applyGravityToPlayer(player);

            // draws all the blocks
            for (int b = 0; b < block_len; b++) {
                SDL_RenderCopy(rend, blocks[b]->tex, NULL, &blocks[b]->dest);
                applyCollisionToPlayerVer(player, blocks[b]);
                applyCollisionToPlayerHor(player, blocks[b]);
            }

            // apply limits to player movement
            applyBoundsToPlayer(player);

            // then draws the player over the blocks
            SDL_RenderCopy(rend, player->tex, NULL, &player->dest);
    
            // triggers the double buffers
            // for multiple rendering
            SDL_RenderPresent(rend);
        }
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
