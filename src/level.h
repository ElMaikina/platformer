#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 360
#define WINDOW_RATE 60
#define TILE_SIZE 12
#define LVL_DIR "lvls"

enum TileType {
  NONE,
  PLAYER,
  BLOCK
}; 

typedef struct {
    SDL_Surface* surf;
    SDL_Texture* text;
    SDL_Rect rect;
    Uint32 *tiles;
    Uint32 w, h;
} Level;

Uint32 GetTileFromLevel(Level *l, int x, int y);
void AddTileToLevel(Level *l, int x, int y, int type);
char* GetLevelFilePath(const char* name);
char* GetLevelFileName(int index);
void GetLevelSizeFromFile(const char* filename, Uint32 *w, Uint32 *h);
Uint32 *LoadLevelFromFile(const char* filename, Uint32 w, Uint32 h);
Level *CreateLevel(SDL_Renderer* rend, Uint32 w, Uint32 h);
void FreeLevel(Level *l);