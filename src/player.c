#include "player.h"

void MoveCamera(Player *p, Level *l, SDL_Renderer* rend) {
    int U = WINDOW_H / 2;
    int L = WINDOW_W / 2;
    int R = l->w*TILE_SIZE - L;
    int D = l->h*TILE_SIZE - U;
    p->ofsx = WINDOW_W / 2;
    p->ofsy = WINDOW_H / 2;
    if (p->x < L)
        p->ofsx += p->x - L;
    if (p->x > R)
        p->ofsx += p->x - R;
    if (p->y < U)
        p->ofsy += p->y - U;
    if (p->y > D)
        p->ofsy += p->y - D;
}

void DrawPlayer(Player *p, Level *l, SDL_Renderer* rend) {
    p->rect.x = p->ofsx;
    p->rect.y = p->ofsy;
    SDL_RenderCopy(rend, p->sprite, NULL, &p->rect);
}

void DrawLevel(Player *p, Level *l, SDL_Renderer* rend) {
    int px = POS_IN_GRID(p->x);
    int py = POS_IN_GRID(p->y);
    int U = POS_IN_GRID(WINDOW_H / 2);
    int L = POS_IN_GRID(WINDOW_W / 2);
    int R = l->w - L;
    int D = l->h - U;
    if (px < L) px = L;
    if (px > R) px = R;
    if (py < U) py = U;
    if (py > D) py = D;
    for (int y = py + U; y > py - U; --y) {
        for (int x = px - L; x < px + L; ++x) {
            if (GetTileFromLevel(l, x, y) == NONE)
                continue;
            l->rect.x = p->ofsx + x * TILE_SIZE - p->x;
            l->rect.y = p->ofsy + y * TILE_SIZE - p->y;
            if (GetTileFromLevel(l, x, y) == BLOCK) {
                SDL_RenderCopy(rend, l->block_spr, NULL, &l->rect);
            }
            if (GetTileFromLevel(l, x, y) == INCLINE) {
                SDL_RenderCopy(rend, l->incline_spr, NULL, &l->rect);
            }
            if (GetTileFromLevel(l, x, y) == DECLINE) {
                SDL_RenderCopy(rend, l->decline_spr, NULL, &l->rect);
            }
        }
    }
}

void PlayerFalling(Player *p, const Uint8 *key) {
    if (p->vy < FALL_SPEED) {
        p->ay = GRAVITY;
    }
    if (p->vy >= FALL_SPEED) {
        p->vy = FALL_SPEED;
    }
    p->vy += p->ay * p->time;
}

void PlayerRunning(Player *p, const Uint8 *key) {
    if (key[SDL_SCANCODE_RIGHT]) {
        if (p->vx < WALK_SPEED)
            p->ax = ACCELERATION;
        if (p->vx >= WALK_SPEED)
            p->vx = WALK_SPEED;
    }
    if (key[SDL_SCANCODE_LEFT]) {
        if (p->vx > -WALK_SPEED)
            p->ax = -ACCELERATION;
        if (p->vx <= WALK_SPEED)
            p->vx = -WALK_SPEED;
    }
    if (!key[SDL_SCANCODE_RIGHT] && !key[SDL_SCANCODE_LEFT]) {
        if (abs(p->vx) < DECELERATION) {
            p->vx = 0;
            p->ax = 0;
        }
        if (p->vx > 0)
            p->ax = -DECELERATION;
        if (p->vx < 0)
            p->ax = DECELERATION;
    }
    p->vx += p->ax * p->time;
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

void PlayerSlowTime(Player *p, const Uint8 *key) {
    if (!key[SDL_SCANCODE_C]) {
        p->time = 1.0;
    }
    if (key[SDL_SCANCODE_C]) {
        p->time = SLOWTIME;
    }
}

void PlayerCollideH(Player *p, Level *l) {
    SDL_Rect res;
    SDL_bool col;
    int px = POS_IN_GRID(p->x);
    int y = POS_IN_GRID(p->y);
    p->rect.x = p->x + round(p->vx * p->time);
    for (int x = px - 2; x < px + 2; ++x) {
        if (GetTileFromLevel(l, x, y) == BLOCK) {
            l->rect.x = x * TILE_SIZE;
            l->rect.y = y * TILE_SIZE;
            col = SDL_IntersectRect(&p->rect, &l->rect, &res);
            if (col == SDL_TRUE) {
                if (p->vx > 0) {
                    p->x = l->rect.x - p->rect.w;
                    p->rect.x = p->x;
                    p->rwall = true;
                    p->vx = 0;
                    p->ax = 0;
                }
                if (p->vx < 0) {
                    p->x = l->rect.x + l->rect.w;
                    p->rect.x = p->x;
                    p->lwall = true;
                    p->vx = 0;
                    p->ax = 0;
                }
            }
        }
    }
    p->rect.x = p->x;
}

void PlayerCollideV(Player *p, Level *l) {
    SDL_Rect res;
    SDL_bool col;
    int x = POS_IN_GRID(p->x);
    int py = POS_IN_GRID(p->y);
    p->rect.y = p->y + round(p->vy * p->time);
    for (int y = py + 2; y > py - 2; --y) {
        if (GetTileFromLevel(l, x, y) == BLOCK) {
            l->rect.x = x * TILE_SIZE;
            l->rect.y = y * TILE_SIZE;
            col = SDL_IntersectRect(&p->rect, &l->rect, &res);
            if (col == SDL_TRUE) {
                if (p->vy > 0) {
                    p->y = l->rect.y - p->rect.h;
                    p->rect.y = p->y;
                    p->floor = true;
                    p->vy = 0;
                    p->ay = 0;
                }
                if (p->vy < 0) {
                    p->y = l->rect.y + l->rect.h;
                    p->rect.y = p->y;
                    p->ceil = true;
                    p->vy = 0;
                    p->ay = 0;
                }
            }
        }
    }
    if (GetTileFromLevel(l, x, py + 1) == BLOCK) {
        if (p->vy >= 0) {
            p->y = l->rect.y - p->rect.h;
            p->rect.y = p->y;
            p->floor = true;
            p->vy = 0;
            p->ay = 0;
        }
        p->rect.y = p->y;
    }
}

void PlayerInSlope(Player *p, Level *l) {
    if (!p->floor) {
        SDL_bool col;
        int px = POS_IN_GRID(p->x);
        int py = POS_IN_GRID(p->y);
        p->rect.y = p->y + round(p->vy * p->time);
        for (int y = py + 1; y > py - 1; --y) {
            for (int x = px + 1; x > px - 1; --x) {
                if (GetTileFromLevel(l, x, y) == INCLINE) {
                    int x1 = x * TILE_SIZE;
                    int y1 = (y + 1) * TILE_SIZE;
                    int x2 = (x + 1) * TILE_SIZE;
                    int y2 = y * TILE_SIZE;
                    col = SDL_IntersectRectAndLine(&p->rect, &x1, &y1, &x2, &y2);
                    if (p->vy > 0 || col == SDL_TRUE) {
                        p->y = y1 - (p->x - x1) - TILE_SIZE * 2 + 5;
                        p->rect.y = p->y;
                        p->floor = true;
                        p->vy = 0;
                        p->ay = 0;
                    }
                }
                if (GetTileFromLevel(l, x, y) == DECLINE) {
                    int x1 = x * TILE_SIZE;
                    int y1 = y * TILE_SIZE;
                    int x2 = (x + 1) * TILE_SIZE;
                    int y2 = (y + 1) * TILE_SIZE;
                    col = SDL_IntersectRectAndLine(&p->rect, &x1, &y1, &x2, &y2);
                    if (p->vy > 0 || col == SDL_TRUE) {
                        p->y = y1 + (p->x - x1) - TILE_SIZE + 5;
                        p->rect.y = p->y;
                        p->floor = true;
                        p->vy = 0;
                        p->ay = 0;
                    }
                }
            }
        }
        p->rect.y = p->y;
    }
}

void PlayerCollision(Player *p, Level *l) {
    p->rwall = false;
    p->lwall = false;
    p->floor = false;
    p->ceil = false;
    PlayerCollideV(p, l);
    PlayerCollideH(p, l);
    PlayerInSlope(p, l);
}

void PlayerApplyMove(Player *p) {
    p->vx += p->ax * p->time;
    p->vy += p->ay * p->time;
    p->x += round(p->vx * p->time);
    p->y += round(p->vy * p->time);
    p->rect.x = p->x;
    p->rect.y = p->y;
}

void MovePlayer(Player *p, Level *l) {
    const Uint8* key = SDL_GetKeyboardState(NULL);
    PlayerApplyMove(p);
    PlayerFalling(p, key);
    PlayerRunning(p, key);
    PlayerJumping(p, key);
    PlayerSlowTime(p, key);
    PlayerCollision(p, l);
}

Player *CreatePlayer(SDL_Renderer* rend, int x, int y) {
    SDL_Rect rect = {x, y, TILE_SIZE, TILE_SIZE};
    Player *p = (Player*)malloc(sizeof(Player));
    p->sprite = IMG_LoadTexture(rend, "img/player.png");
    p->rect = rect;
    p->time = 1.0;
    p->x = x;
    p->y = y;
    return p;
}

void FreePlayer(Player *p) {
    SDL_DestroyTexture(p->sprite);
    free(p);
}
