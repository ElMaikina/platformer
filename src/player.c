#include "../include/player.h"

Player *createPlayer(SDL_Renderer* rend, char sprite[]) {
    // creates the player pointer and allocates memory
    Player *player;
    player = (Player*)malloc(sizeof(Player));

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
    dest.x = (WINDOW_WIDTH - dest.w) / 2;
    dest.y = (WINDOW_HEIGHT - dest.h) / 2;

    // assigns sprite properties
    player->surface = surface;
    player->tex = tex;
    player->dest = dest;

    // gameplay parameters
    player->vel_x = 0;
    player->vel_y = 0;
    
    // gameplay states
    player->wall_right = false;
    player->wall_left = false;
    player->wall_top = false;
    player->jumped = false;
    player->ground = false;
    player->control = true;

    return player;
}

void moveAndControlPlayer(Player *player) {
    // gets pressed keys
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    
    // if player is in controllable state
    // then react to keyboard events
    if (player->control) {
        // uses the arrow keys for movement
        if (!player->wall_left && !player->wall_right) 
        {
            if (keystate[SDL_SCANCODE_LEFT]) {
                if (!keystate[SDL_SCANCODE_Z])
                    player->vel_x = -PLAYER_WALK_SPEED;
                if (keystate[SDL_SCANCODE_Z])
                    player->vel_x = -PLAYER_RUN_SPEED;
            }
            if (keystate[SDL_SCANCODE_RIGHT]) {
                if (!keystate[SDL_SCANCODE_Z])
                    player->vel_x = PLAYER_WALK_SPEED;
                if (keystate[SDL_SCANCODE_Z])
                    player->vel_x = PLAYER_RUN_SPEED;
            }
            // if the player is not pressing 
            // right or left, stop moving
            if (!keystate[SDL_SCANCODE_LEFT] && !keystate[SDL_SCANCODE_RIGHT])
                player->vel_x = 0;
        }

        // jump when on the ground
        if (player->ground && !player->jumped) {
            if (keystate[SDL_SCANCODE_X]) {
                player->vel_y = -PLAYER_JUMP_SPEED;
                player->jumped = true;
            }
        }
        // release the key first to keep jumping
        if (!keystate[SDL_SCANCODE_X]) {
            player->jumped = false;
        }
    }
    // apply player speed to sprite
    player->dest.x += player->vel_x;
    player->dest.y += player->vel_y;

    // reset collision state to be 
    // overwritten by collision function
    player->wall_right = false;
    player->wall_left = false;
    player->wall_top = false;
    player->ground = false;
}

void applyBoundsToPlayer(Player *player) {
    // limit player to the window limits
    if (player->dest.x + player->vel_x < 0)
        player->dest.x = 0;

    if (player->dest.x + player->dest.w + player->vel_x > WINDOW_WIDTH)
        player->dest.x = WINDOW_WIDTH - player->dest.w;
    
    if (player->dest.y + player->vel_y < 0)
        player->dest.y = 0;
        
    if (player->dest.y + player->dest.h + player->vel_y > WINDOW_HEIGHT) {
        player->dest.y = WINDOW_HEIGHT - player->dest.h;
        player->ground = true;
        player->vel_y = 0;
    }
}

void applyGravityToPlayer(Player *player) {
    // apply gravity to player
    if (player->vel_y < PLAYER_GRAVITY) {
        player->vel_y += 1;
    }
    if (player->vel_y >= PLAYER_GRAVITY) {
        player->vel_y = PLAYER_GRAVITY;
    }
}

void applyCollisionToPlayerVer(Player *player, Block *block) {
    // check intersection between player and block
    if (player->dest.x < block->dest.x + block->dest.w &&
        player->dest.x + player->dest.w > block->dest.x &&
        player->dest.y + player->vel_y < block->dest.y + block->dest.h &&
        player->dest.y + player->vel_y + player->dest.h > block->dest.y) 
        {
        if (player->dest.y < block->dest.y && player->vel_y > 0) {
            player->dest.y = block->dest.y - player->dest.h;
            player->ground = true;
            player->vel_y = 0;
            return;
        }
        if (player->dest.y > block->dest.y && player->vel_y < 0) {
            player->dest.y = block->dest.y + player->dest.h;
            player->wall_top = true;
            player->vel_y = 0;
            return;
        }
        return;
    }
}

void applyCollisionToPlayerHor(Player *player, Block *block) {
    // check intersection between player and block
    if (player->dest.x + player->vel_x < block->dest.x + block->dest.w &&
        player->dest.x + player->vel_x + player->dest.w > block->dest.x &&
        player->dest.y < block->dest.y + block->dest.h &&
        player->dest.y + player->dest.h > block->dest.y)
        {
        if (player->dest.x < block->dest.x && player->vel_x > 0) {
            player->dest.x = block->dest.x - player->dest.w;
            player->wall_right = true;
            player->vel_x = 0;
            return;
        }
        if (player->dest.x > block->dest.x && player->vel_x < 0) {
            player->dest.x = block->dest.x + block->dest.w;
            player->wall_left = true;
            player->vel_x = 0;
            return;
        }
        return;
    }
}