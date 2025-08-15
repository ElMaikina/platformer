#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "level.h"

#define WALK_SPEED 4.0
#define RUN_SPEED 8.0
#define WALLJUMP_H 10.0
#define WALLJUMP_V 8.0
#define ACCELERATION 1.0
#define DECELERATION 0.5
#define JUMP_SPEED 8.0
#define FALL_SPEED 8.0
#define GRAVITY 0.5
#define SLOWTIME 0.2

typedef struct {
    SDL_Surface* surf;
    SDL_Texture* text;
    SDL_Rect rect;
    int x, y, ofsx, ofsy;
    float vx, vy, ax, ay;
    float speed, time;
    bool rwall, lwall;
    bool floor, ceil;
    bool jump;
} Player;

Player *CreatePlayer(SDL_Renderer* rend, int x, int y);
void DrawPlayer(Player *p, Level *l, SDL_Renderer* rend);
void DrawLevel(Player *p, Level *l, SDL_Renderer* rend);
void MoveCamera(Player *p, Level *l, SDL_Renderer* rend);
void MovePlayer(Player *p, Level *l);
void FreePlayer(Player *p);