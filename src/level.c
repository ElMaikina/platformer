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

void GetLevelSizeFromFile(const char* filename, Level *l) {
    const char* dims = strrchr(filename, '-');
    if (!dims) 
        return;
    dims++;
    const char* x = strchr(dims, 'x');
    if (!x) 
        return;
    l->w = (Uint32)atoi(dims);
    l->h = (Uint32)atoi(x + 1);
}

void LoadLevelFromFile(const char* path, Level *l) {
    FILE* f = fopen(path, "r");
    if (!f) {
        perror("Error opening level");
        return;
    }
    l->tiles = (Uint32 *)malloc(l->w * l->h * sizeof(Uint32));
    for (Uint32 y = 0; y < l->h; ++y) {
        for (Uint32 x = 0; x < l->w; ++x) {
            if (fscanf(f, "%u", &l->tiles[y * l->w + x]) != 1) {
                fprintf(stderr, "Error reading CSV at %d,%d\n", y, x);
                fclose(f);
                return;
            }
            if (x < l->w - 1) {
                fgetc(f);
            }
        }
        fgetc(f);
    }
    fclose(f);
}

Level *CreateLevel(SDL_Renderer* rend, int i) {
    SDL_Rect rect = {0, 0, TILE_SIZE, TILE_SIZE};
    Level *l = malloc(sizeof(Level));
    char *name = GetLevelFileName(i);
    char *path = GetLevelFilePath(name);
    l->block_spr = IMG_LoadTexture(rend, "img/block.png");
    l->incline_spr = IMG_LoadTexture(rend, "img/incline.png");
    l->decline_spr = IMG_LoadTexture(rend, "img/decline.png");
    l->up_spr = IMG_LoadTexture(rend, "img/up.png");
    GetLevelSizeFromFile(name, l);
    LoadLevelFromFile(path, l);
    free(path);
    free(name);
    l->rect = rect;
    return l;
}

void FreeLevel(Level *l) {
    SDL_DestroyTexture(l->block_spr);
    SDL_DestroyTexture(l->incline_spr);
    SDL_DestroyTexture(l->decline_spr);
    SDL_DestroyTexture(l->up_spr);
    free(l->tiles);
    free(l);
}
