#include "level.h"

Uint32 GetTileFromLevel(Level *l, int x, int y) {
    if (y >= 0 && y < l->h && x >= 0 && x < l->w)
        return l->tiles[y * l->w + x];
    return 0;
}

void AddTileToLevel(Level *l, int x, int y, int type) {
    l->tiles[y * l->w + x] = type;
}

char* GetLevelFileName(int index) {
    DIR* dir = opendir(LVL_DIR);
    if (!dir) {
        perror("Error opening levels directory");
        return NULL;
    }
    struct dirent* entry;
    char match_prefix[32];
    snprintf(match_prefix, sizeof(match_prefix), "%d-", index);

    char* found_filename = NULL;
    int matches = 0;

    while ((entry = readdir(dir)) != NULL) {
        const char* ext = strrchr(entry->d_name, '.');
        if (!ext || strcmp(ext, ".csv") != 0) {
            continue;
        }
        if (strncmp(entry->d_name, match_prefix, strlen(match_prefix)) == 0) {
            matches++;
            if (matches == 1) {
                found_filename = malloc(strlen(entry->d_name) + 1);
                strcpy(found_filename, entry->d_name);
            }
            else {
                free(found_filename);
                closedir(dir);
                return NULL;
            }
        }
    }
    closedir(dir);
    if (matches != 1) {
        free(found_filename);
        return NULL;
    }
    return found_filename;
}

char* GetLevelFilePath(const char* name) {
    char *path = malloc((strlen(LVL_DIR) + strlen(name) + 1) * sizeof(char));
    strcpy(path, "");
    strcat(path, LVL_DIR);
    strcat(path, "/");
    strcat(path, name);
    return path;
}

void GetLevelSizeFromFile(const char* filename, Uint32 *w, Uint32 *h) {
    const char* dims = strrchr(filename, '-');
    if (!dims) 
        return;
    dims++;

    const char* x = strchr(dims, 'x');
    if (!x) 
        return;

    *w = (Uint32)atoi(dims);
    *h = (Uint32)atoi(x + 1);
}

Uint32 *LoadLevelFromFile(const char* path, Uint32 w, Uint32 h) {
    FILE* f = fopen(path, "r");
    if (!f) {
        perror("Error opening level");
        return NULL;
    }
    Uint32 *tiles = (Uint32 *)malloc(w * h * sizeof(Uint32));
    for (Uint32 y = 0; y < h; ++y) {
        for (Uint32 x = 0; x < w; ++x) {
            if (fscanf(f, "%d", &tiles[y * w + x]) != 1) {
                fprintf(stderr, "Error reading CSV at %d,%d\n", y, x);
                fclose(f);
                return NULL;
            }
            if (x < w - 1) {
                fgetc(f);
            }
        }
        fgetc(f);
    }
    fclose(f);
    return tiles;
}

Level *CreateLevel(SDL_Renderer* rend, int i) {
    SDL_Rect rect;
    Level *level = malloc(sizeof(Level));

    SDL_Surface* blocksurf = IMG_Load("img/block.png");
    SDL_Texture* blocktext = SDL_CreateTextureFromSurface(rend, blocksurf);

    SDL_Surface* slopesurf = IMG_Load("img/slope.png");
    SDL_Texture* slopetext = SDL_CreateTextureFromSurface(rend, blocksurf);

    SDL_QueryTexture(blocktext, NULL, NULL, &rect.w, &rect.h);
    SDL_QueryTexture(slopetext, NULL, NULL, &rect.w, &rect.h);

    rect.x = 0; rect.y = 0;
    level->blocksurf = blocksurf;
    level->slopesurf = slopesurf;
    level->blocktext = blocktext;
    level->slopetext = slopetext;
    level->rect = rect;

    Uint32 w, h;
    char *name = GetLevelFileName(i);
    GetLevelSizeFromFile(name, &w, &h);
    char *path = GetLevelFilePath(name);
    Uint32 *tiles = LoadLevelFromFile(path, w, h);
    level->tiles = tiles;
    level->w = w;
    level->h = h;

    free(path);
    free(name);
    return level;
}

void FreeLevel(Level *l) {
    SDL_DestroyTexture(l->blocktext);
    SDL_DestroyTexture(l->slopetext);
    SDL_FreeSurface(l->blocksurf);
    SDL_FreeSurface(l->slopesurf);
    free(l->tiles);
    free(l);
}
