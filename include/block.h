typedef struct {
    // sprite properties
    SDL_Surface* surface;
    SDL_Texture* tex;
    SDL_Rect dest;
} Block;

// creates the player
Block *createBlock(SDL_Renderer* rend, int x, int y, char sprite[]);