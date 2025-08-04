#include "level.h"

Level *CreateLevel(SDL_Renderer* rend, Uint32 w, Uint32 h) {
    SDL_Rect rect;
    Level *level = malloc(sizeof(Level));
    SDL_Surface* surf = IMG_Load("img/block.png");
    SDL_Texture* text = SDL_CreateTextureFromSurface(rend, surf);
    SDL_QueryTexture(text, NULL, NULL, &rect.w, &rect.h);
    rect.x = 0; rect.y = 0;
    level->surf = surf;
    level->text = text;
    level->rect = rect;
    level->w = w;
    level->h = h;
    Uint32 *tiles = (Uint32 *)malloc(w * h * sizeof(Uint32));
    for (Uint32 i = 0; i < h; ++i) {
        for (Uint32 j = 0; j < w; ++j) {
            tiles[i * w + j] = 0;
        }
    }
    level->tiles = tiles;
    return level;
}

void FreeLevel(Level *l) {
    SDL_DestroyTexture(l->text);
    SDL_FreeSurface(l->surf);
    free(l->tiles);
    free(l);
}

Uint32 GetTileFromLevel(Level *l, int x, int y) {
    int tile = l->tiles[y * l->w + x];
    return tile;
}

void AddTileToLevel(Level *l, int x, int y) {
    l->tiles[y * l->w + x] = 1;
}
