#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include <SDL3/SDL.h>
#include <glm.hpp>

struct Circle {
    float radius;
    glm::vec2 pos;
    SDL_Color color;

    Circle();
    Circle(float r, glm::vec2 c, SDL_Color color);

    void draw(SDL_Renderer* renderer);
};

void plotPoints(SDL_Renderer *renderer, int x, int y, Circle circle);
void plotLines(SDL_Renderer *renderer, int x, int y, Circle circle);
void DrawCircle(SDL_Renderer *renderer, Circle circle);
void FillCircle(SDL_Renderer *renderer, Circle circle);

#endif //CIRCLE_HPP