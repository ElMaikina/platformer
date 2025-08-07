#include "player.h"

Player *CreatePlayer(SDL_Renderer* rend, int x, int y) {
    SDL_Rect rect;
    Player *player = (Player*)malloc(sizeof(Player));
    SDL_Surface* surf = IMG_Load("img/player.png");
    SDL_Texture* text = SDL_CreateTextureFromSurface(rend, surf);
    SDL_QueryTexture(text, NULL, NULL, &rect.w, &rect.h);
    rect.x = x; rect.y = y;
    player->surf = surf;
    player->text = text;
    player->rect = rect;
    player->rwall = false;
    player->lwall = false;
    player->floor = false;
    player->ceil = false;
    player->jump = false;
    player->time = 1.0;
    player->speed = 0;
    player->x = x;
    player->y = y;
    player->vx = 0;
    player->vy = 0;
    player->ofsx = 0;
    player->ofsy = 0;
    return player;
}

void MoveCamera(Player *p, Level *l, SDL_Renderer* rend) {
    int top = WINDOW_HEIGHT / 2;
    int left = WINDOW_WIDTH / 2;
    int right = l->w*TILE_SIZE - left;
    int btm = l->h*TILE_SIZE - top;
    p->ofsx = -p->rect.w / 2;
    p->ofsy = -p->rect.h / 2;
    p->ofsx += WINDOW_WIDTH / 2;
    p->ofsy += WINDOW_HEIGHT / 2;
    if (p->x < left) {
        p->ofsx += p->x - left;
    }
    if (p->x > right) {
        p->ofsx += p->x - right;
    }
    if (p->y < top) {
        p->ofsy += p->y - top;
    }
    if (p->y > btm) {
        p->ofsy += p->y - btm;
    }
}

void DrawPlayer(Player *p, Level *l, SDL_Renderer* rend) {
    p->rect.x = p->ofsx;
    p->rect.y = p->ofsy;
    SDL_RenderCopy(rend, p->text, NULL, &p->rect);
}

void DrawLevel(Player *p, Level *l, SDL_Renderer* rend) {
    int px = p->x / TILE_SIZE;
    int py = p->y / TILE_SIZE;
    int wr = (WINDOW_WIDTH / 2) / TILE_SIZE + 2;
    int hr = (WINDOW_HEIGHT / 2) / TILE_SIZE + 2;
    for (int y = py + hr; y > py - hr; --y) {
        for (int x = px - wr; x < px + wr; ++x) {
            if (y >= 0 && y < l->h && x >= 0 && x < l->w) {
                if (GetTileFromLevel(l, x, y) == BLOCK) {
                    l->rect.x = p->ofsx + x * TILE_SIZE - p->x;
                    l->rect.y = p->ofsy + y * TILE_SIZE - p->y;
                    l->rect.w = TILE_SIZE;
                    l->rect.h = TILE_SIZE;
                    SDL_RenderCopy(rend, l->text, NULL, &l->rect);
                }
            }
        }
    }
}

void FreePlayer(Player *p) {
    SDL_DestroyTexture(p->text);
    SDL_FreeSurface(p->surf);
    free(p);
}

void PlayerFalling(Player *p, const Uint8 *key) {
    if (p->vy < FALL_SPEED) {
        p->ay = GRAVITY;
    }
    if (p->vy >= FALL_SPEED) {
        p->vy = FALL_SPEED;
    }
}

void PlayerRunning(Player *p, const Uint8 *key) {
    if (key[SDL_SCANCODE_RIGHT]) {
        if (abs(p->speed - p->vx) < DECELERATION) {
            p->vx = p->speed;
        }
        if (p->vx < p->speed) {
            p->ax = ACCELERATION;
        }
        if (p->vx > p->speed) {
            p->ax = -DECELERATION;
        }
    }
    if (key[SDL_SCANCODE_LEFT]) {
        if (abs(p->speed + p->vx) < DECELERATION) {
            p->vx = -p->speed;
        }
        if (p->vx > -p->speed) {
            p->ax = -ACCELERATION;
        }
        if (p->vx < -p->speed) {
            p->ax = DECELERATION;
        }
    }
    if (!key[SDL_SCANCODE_RIGHT] && !key[SDL_SCANCODE_LEFT]) {
        if (p->vx > 0) {
            p->ax = -DECELERATION;
        }
        if (p->vx < 0) {
            p->ax = DECELERATION;
        }
        if (abs(p->vx) < DECELERATION) {
            p->vx = 0;
        }
    }
}

void PlayerJumping(Player *p, const Uint8 *key) {
    if (key[SDL_SCANCODE_X] && !p->jump) {
        if (p->floor) {
            p->vy = -JUMP_SPEED;
            p->jump = true;
        }
        if (!p->floor && p->rwall) {
            p->vx = -WALLJUMP_H;
            p->vy = -WALLJUMP_V;
            p->jump = true;
        }
        if (!p->floor && p->lwall) {
            p->vx = WALLJUMP_H;
            p->vy = -WALLJUMP_V;
            p->jump = true;
        }
    }
    if (!key[SDL_SCANCODE_X]) {
        p->jump = false;
    }
}

void PlayerMaxSpeed(Player *p, const Uint8 *key) {
    if (!key[SDL_SCANCODE_Z]) {
        p->speed = WALK_SPEED;
    }
    if (key[SDL_SCANCODE_Z]) {
        p->speed = RUN_SPEED;
    }
}

void PlayerSlowTime(Player *p, const Uint8 *key) {
    if (!key[SDL_SCANCODE_C]) {
        p->time = 1.0;
    }
    if (key[SDL_SCANCODE_C]) {
        p->time = SLOWTIME;
    }
}

void PlayerCollideLeftRight(Player *p, Level *l) {
    SDL_Rect res;
    SDL_bool col;
    int px = p->x / TILE_SIZE;
    int py = p->y / TILE_SIZE;
    for (int y = py + 2; y > py - 2; --y) {
        for (int x = px - 2; x < px + 2; ++x) {
            if (y >= 0 && y < l->h && x >= 0 && x < l->w) {
                if (GetTileFromLevel(l, x, y) == BLOCK) {
                    l->rect.x = x * TILE_SIZE;
                    l->rect.y = y * TILE_SIZE;
                    p->rect.x = p->x + round(p->vx);
                    col = SDL_IntersectRect(&p->rect, &l->rect, &res);
                    if (col == SDL_TRUE) {
                        if (p->vx > 0) {
                            p->x = l->rect.x - p->rect.w;
                            p->rwall = true;
                            p->vx = 0;
                        }
                        if (p->vx < 0) {
                            p->x = l->rect.x + l->rect.w;
                            p->lwall = true;
                            p->vx = 0;
                        }
                    }
                }
            }
        }
    }
}

bool PlayerOverBlock(Player *p, Level *l) {
    int dx = p->x - l->rect.x;
    int dy = p->y - l->rect.y;
    bool fall = p->vy > 0;
    bool over = dy < 0 && abs(dy) > abs(dx);
    bool result = fall && over;
    return result;
}
bool PlayerUnderBlock(Player *p, Level *l) {
    int dx = p->x - l->rect.x;
    int dy = p->y - l->rect.y;
    bool rise = p->vy < 0;
    bool under = dy > 0 && abs(dy) > abs(dx);
    bool result = rise && under;
    return result;
}

void PlayerCollideTopBottom(Player *p, Level *l) {
    SDL_Rect res;
    SDL_bool col;
    Uint32 h = p->rect.h;
    int px = p->x / TILE_SIZE;
    int py = p->y / TILE_SIZE;
    for (int y = py + 2; y > py - 2; --y) {
        for (int x = px - 2; x < px + 2; ++x) {
            if (y >= 0 && y < l->h && x >= 0 && x < l->w) {
                if (GetTileFromLevel(l, x, y) == BLOCK) {
                    l->rect.x = x * TILE_SIZE;
                    l->rect.y = y * TILE_SIZE;
                    p->rect.h = h + round(p->vy);
                    col = SDL_IntersectRect(&p->rect, &l->rect, &res);
                    if (col == SDL_TRUE) {
                        p->rect.h = h;
                        if (PlayerOverBlock(p, l)) {
                            p->y = l->rect.y - p->rect.h;
                            p->rect.y = p->y;
                            p->floor = true;
                            p->vy = 0;
                        }
                        if (PlayerUnderBlock(p, l)) {
                            p->y = l->rect.y + l->rect.h;
                            p->rect.y = p->y;
                            p->ceil = true;
                            p->vy = 0;
                        }
                    }
                }
            }
        }
    }
    p->rect.h = h;
}

void PlayerCollision(Player *p, Level *l) {
    PlayerCollideTopBottom(p, l);
    PlayerCollideLeftRight(p, l);
}

void PlayerApplyMove(Player *p) {
    p->vx += p->ax * p->time;
    p->vy += p->ay * p->time;
    p->x += round(p->vx * p->time);
    p->y += round(p->vy * p->time);
    p->rect.x = p->x;
    p->rect.y = p->y;
}

void PlayerResetBounds(Player *p) {
    p->rwall = false;
    p->lwall = false;
    p->floor = false;
    p->ceil = false;
}

void MovePlayer(Player *p, Level *l) {
    const Uint8* key = SDL_GetKeyboardState(NULL);
    PlayerFalling(p, key);
    PlayerRunning(p, key);
    PlayerJumping(p, key);
    PlayerMaxSpeed(p, key);
    PlayerSlowTime(p, key);
    PlayerApplyMove(p);
    PlayerResetBounds(p);
    PlayerCollision(p, l);
}