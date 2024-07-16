#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdbool.h>

#include "block.h"
#include "config.h"

typedef struct {
    // sprite properties
    SDL_Surface* surface;
    SDL_Texture* tex;
    SDL_Rect dest;
    
    // gameplay parameters
    int vel_x;
    int vel_y;
    
    // gameplay states
    bool has_control;
    bool has_jumped;
    bool on_ground;
} Player;

// creates the player
Player *createPlayer(SDL_Renderer* rend, char sprite[]);

// manages player movement
void moveAndControlPlayer(Player *player);

// limits player movement inside level margin
void applyBoundsToPlayer(Player *player);

// apply world gravity to player
void applyGravityToPlayer(Player *player);

// limits player movement vertically
void applyCollisionToPlayerVer(Player *player, Block *block);

// limits player movement horizontally
void applyCollisionToPlayerHor(Player *player, Block *block);
