#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdbool.h>

// check if two rectangles are intersecting each other
bool areRectsColliding(SDL_Rect rect1, SDL_Rect rect2);

// collide player with inmovable rectangle horizontally
int playerCollideHor(SDL_Rect rect1, SDL_Rect rect2);

// collide player with inmovable rectangle vertically
int playerCollideVer(SDL_Rect rect1, SDL_Rect rect2);