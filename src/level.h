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
    SDL_Surface* blocksurf;
    SDL_Texture* blocktext;
    SDL_Surface* inclinesurf;
    SDL_Texture* inclinetext;
    SDL_Surface* declinesurf;
    SDL_Texture* declinetext;
    Uint32 *tiles;
    Uint32 w, h;
} Level;

Uint32 GetTileFromLevel(Level *l, int x, int y);
void AddTileToLevel(Level *l, int x, int y, int type);
char* GetLevelFilePath(const char* name);
char* GetLevelFileName(int index);
void GetLevelSizeFromFile(const char* filename, Uint32 *w, Uint32 *h);
Uint32 *LoadLevelFromFile(const char* filename, Uint32 w, Uint32 h);
Level *CreateLevel(SDL_Renderer* rend, int i);
void FreeLevel(Level *l);
