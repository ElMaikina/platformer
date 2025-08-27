#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

#define WINDOW_W 640
#define WINDOW_H 360
#define WINDOW_RATE 60
#define TILE_SIZE 12
#define LVL_DIR "lvls"

#define POS_IN_GRID(N)  (((N) == 0) ? 0:(((N * 10)/TILE_SIZE) + 5)/10)

enum TileType {
  NONE,
  PLAYER,
  BLOCK,
  INCLINE,
  DECLINE
}; 

typedef struct {
    SDL_Rect rect;
    SDL_Texture* block_spr;
    SDL_Texture* incline_spr;
    SDL_Texture* decline_spr;
    Uint32 *tiles;
    Uint32 w, h;
} Level;

Uint32 GetTileFromLevel(Level *l, int x, int y);
void AddTileToLevel(Level *l, int x, int y, int type);
char* GetLevelFilePath(const char* name);
char* GetLevelFileName(int index);
void GetLevelSizeFromFile(const char* filename, Level *l);
void LoadLevelFromFile(const char* filename, Level *l);
Level *CreateLevel(SDL_Renderer* rend, int i);
void FreeLevel(Level *l);
