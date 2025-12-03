#ifndef BRICK_HPP
#define BRICK_HPP

#include <SDL3/SDL.h>
#include "const.hpp" 

class Brick {
private:
    SDL_FRect rect;
    SDL_Color color;
    int health;

public:
    Brick(float x, float y, float h, float w, int health) {
        rect.x = x;
        rect.y = y;
        rect.h = h;
        rect.w = w;
        this->health = health;
        getupdate();
    }

    SDL_FRect getrect(void) const { return this->rect; }
    SDL_Color getcolor(void) const { return this->color; }
    int gethealth(void) const { return this->health; }

    void gethit() {
        if (health > 0) {
            health--;
            getupdate();
        }
    }

    void getupdate() {
        switch (health) {
            case 1: color = {255, 0, 0, 255}; break;    //Red
            case 2: color = {255, 255, 0, 255}; break;  //Yellow
            case 3: color = {0, 255, 0, 255}; break;    //Green
            default: color = {100, 100, 100, 255}; break;
        }
    }

    void draw(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &rect);
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderRect(renderer, &rect);
    }
};

#endif // BRICK_HPP