#include "../include/physics.h"

bool areRectsColliding(SDL_Rect rect1, SDL_Rect rect2) {
    if (rect1.x < rect2.x + rect2.w &&
        rect1.x + rect1.w > rect2.x &&
        rect1.y < rect2.y + rect2.h &&
        rect1.y + rect1.h > rect2.y) {
        return true;
    }
    return false;
}

int playerCollideHor(SDL_Rect rect1, SDL_Rect rect2) {
    if (areRectsColliding(rect1, rect2)) {
        if (rect1.x < rect2.x + rect2.w)
            return rect2.x + rect2.w;
        if (rect1.x + rect1.w > rect2.x)
            return rect2.x - rect1.w;
    }
    return rect1.x;
}

int playerCollideVer(SDL_Rect rect1, SDL_Rect rect2) {
    if (areRectsColliding(rect1, rect2)) {
        if (rect1.y < rect2.y + rect1.h)
            return rect2.y + rect2.h;
        if (rect1.y + rect1.h > rect2.y)
            return rect2.y - rect1.h;
    }
    return rect1.y;
}