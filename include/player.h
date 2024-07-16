typedef struct {
    // sprite properties
    SDL_Surface* surface;
    SDL_Texture* tex;
    SDL_Rect dest;
    
    // gameplay parameters
    int vel_x;
    int vel_y;
    
    // gameplay states
    bool has_control;
    bool has_jumped;
    bool on_ground;
} Player;

// creates the player
Player *createPlayer(SDL_Renderer* rend, char sprite[]);

// manages player movement
void moveAndControlPlayer(Player *player);

// limits player movement
void applyBoundsToPlayer(Player *player);

// apply world gravity to player
void applyGravityToPlayer(Player *player);