#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <iostream>
#include <string> 
#include <glm.hpp> 

#include "const.hpp"
#include "circle.hpp"
#include "brick.hpp"

//game states for easier handling
enum class GameState {
    START,      //ball locked to paddle
    PLAYING,    //ball moving
    GAME_OVER,  //lost, paddle turns red
    GAME_WON    //won, paddle turns green
};

class Game {
public:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool isRunning = false;
    GameState state;

    Circle ball;
    glm::vec2 ballVel;
    SDL_FRect paddle;
    std::vector<Brick> bricks;

    Game() {
        //start in START mode
        state = GameState::START;

        //initializing
        ball = Circle(BALL_RADIUS, glm::vec2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f), {241, 90, 34, 255});
        ballVel = glm::vec2(0, 0); 
        paddle = { (SCREEN_WIDTH / 2.0f - PADDLE_WIDTH / 2.0f), PADDLE_Y, PADDLE_WIDTH, PADDLE_HEIGHT };
    }

    ~Game() {
        clean();
    }

    void showErrorMessage(const char* title, const char* message) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, message, NULL);
    }

    void init(const char* title, int width, int height, bool fullscreen) {
        //handled window resizing
        SDL_WindowFlags flags = SDL_WINDOW_RESIZABLE;
        if (fullscreen) flags |= SDL_WINDOW_FULLSCREEN;

        //error checking
        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
            std::string err = "SDL_Init Failed: ";
            err += SDL_GetError();
            showErrorMessage("Initialization Error", err.c_str());
            isRunning = false;
            return;
        }

        window = SDL_CreateWindow(title, width, height, flags);
        if (!window) {
            std::string err = "SDL_CreateWindow Failed: ";
            err += SDL_GetError();
            showErrorMessage("Window Error", err.c_str());
            isRunning = false;
            return;
        }

        renderer = SDL_CreateRenderer(window, NULL);
        if (!renderer) {
            std::string err = "SDL_CreateRenderer Failed: ";
            err += SDL_GetError();
            showErrorMessage("Renderer Error", err.c_str());
            isRunning = false;
            return;
        }

        //SDL3 automatic scaling
        SDL_SetRenderLogicalPresentation(renderer, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        isRunning = true;

        resetGame();
    }

    void resetGame() {
        state = GameState::START;
        
        paddle.x = (SCREEN_WIDTH - PADDLE_WIDTH) / 2.0f;
        paddle.y = PADDLE_Y;

        ball.pos = glm::vec2(paddle.x + paddle.w / 2.0f, paddle.y - ball.radius);
        ball.color = {241, 90, 34, 255}; 
        
        initBricks();
        std::cout << "Game Reset. Press SPACE to start." << std::endl;
    }

    void initBricks() {
        bricks.clear();
        
        int rows = 3;
        int cols = 17;
        float padding = 4.0f; 
        
     
        float totalPadding = (cols + 1) * padding;
        float availableWidth = (float)SCREEN_WIDTH - totalPadding;
        float brickWidth = availableWidth / cols;
        float brickHeight = 20.0f; 

        float startY = 100.0f;

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                float x = padding + j * (brickWidth + padding);
                float y = startY + i * (brickHeight + padding);
                
                //color/health logic
                int health = 1; 
                if (i == 1) health = 2;
                if (i == 2) health = 3;

                bricks.emplace_back(x, y, brickHeight, brickWidth, health);
            }
        }
    }

    void handleEvents() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                isRunning = false;
            }
            else if (event.type == SDL_EVENT_KEY_DOWN) {
                std::cout << "Key Pressed: " << SDL_GetKeyName(event.key.key) << std::endl;

                if (state == GameState::START) {
                    if (event.key.key == SDLK_SPACE) {
                        state = GameState::PLAYING;
                        ballVel = glm::vec2(2.0f, -BALL_SPEED); 
                        std::cout << "Ball Launched!" << std::endl;
                    }
                }
                else if (state == GameState::GAME_OVER || state == GameState::GAME_WON) {
                    if (event.key.key == SDLK_R) {
                        resetGame();
                    }
                }
            }
        }
        if (state == GameState::START || state == GameState::PLAYING) {
            int numKeys;
            const bool* currentKeyStates = SDL_GetKeyboardState(&numKeys);
            
            if (currentKeyStates) {
                if (currentKeyStates[SDL_SCANCODE_LEFT]) {
                    paddle.x -= PADDLE_SPEED;
                    if (paddle.x < 0) paddle.x = 0;
                }
                if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
                    paddle.x += PADDLE_SPEED;
                    if (paddle.x + PADDLE_WIDTH > SCREEN_WIDTH) paddle.x = SCREEN_WIDTH - PADDLE_WIDTH;
                }
            }
        }
    }

    bool checkCollision(const Circle& c, const SDL_FRect& rect) {
        float closestX = c.pos.x;
        float closestY = c.pos.y;

        if (c.pos.x < rect.x) closestX = rect.x;
        else if (c.pos.x > rect.x + rect.w) closestX = rect.x + rect.w;

        if (c.pos.y < rect.y) closestY = rect.y;
        else if (c.pos.y > rect.y + rect.h) closestY = rect.y + rect.h;

        float distanceX = c.pos.x - closestX;
        float distanceY = c.pos.y - closestY;
        float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);

        return distanceSquared < (c.radius * c.radius);
    }

    void update() {
        if (!isRunning) return;

        if (state == GameState::START) {
            //locking ball to paddle
            ball.pos.x = paddle.x + (paddle.w / 2.0f);
            ball.pos.y = paddle.y - ball.radius - 1.0f;
        }
        else if (state == GameState::PLAYING) {
            ball.pos += ballVel;

            //wall collisions
            if (ball.pos.x - ball.radius < 0) {
                ball.pos.x = ball.radius;
                ballVel.x = -ballVel.x;
            } else if (ball.pos.x + ball.radius > SCREEN_WIDTH) {
                ball.pos.x = SCREEN_WIDTH - ball.radius;
                ballVel.x = -ballVel.x;
            }

            if (ball.pos.y - ball.radius < 0) {
                ball.pos.y = ball.radius;
                ballVel.y = -ballVel.y;
            } 
            //game over condition
            else if (ball.pos.y - ball.radius > SCREEN_HEIGHT) {
                state = GameState::GAME_OVER;
                std::cout << "Game Over! Press R to Restart." << std::endl;
            }

            //paddle collision
            if (checkCollision(ball, paddle)) {
                
                float paddleCenter = paddle.x + paddle.w / 2.0f;
                float hitDist = ball.pos.x - paddleCenter;
                
                
                float halfWidth = paddle.w / 2.0f;
                float normalizedHit = hitDist / halfWidth;
                
                
                if (normalizedHit < -1.0f) normalizedHit = -1.0f;
                if (normalizedHit > 1.0f) normalizedHit = 1.0f;
                
                
                float angleDeg = normalizedHit * 80.0f;
                
                
                float angleRad = glm::radians(angleDeg);
                
                
                ballVel.x = BALL_SPEED * std::sin(angleRad);
                ballVel.y = -BALL_SPEED * std::cos(angleRad);
                
                
                ball.pos.y = paddle.y - ball.radius - 1.0f;
            }

            //brick collision and win check
            bool allDestroyed = true;
            for (auto& b : bricks) {
                if (b.gethealth() > 0) {
                    allDestroyed = false;
                    if (checkCollision(ball, b.getrect())) {
                        b.gethit();
                        ballVel.y = -ballVel.y;
                        break; 
                    }
                }
            }

            //game won cond
            if (allDestroyed) {
                state = GameState::GAME_WON;
                std::cout << "YOU WON! Press R to Restart." << std::endl;
            }
        }
    }

    void render() {
        if (!isRunning || !renderer) return;

        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        //changing paddle color based on game state
        if (state == GameState::GAME_OVER) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); //RED for loss
        } else if (state == GameState::GAME_WON) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); //GREEN for win
        } else {
            SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); //CYAN for playing
        }
        SDL_RenderFillRect(renderer, &paddle);

        //draw bricks
        for (auto& b : bricks) {
            if (b.gethealth() > 0) {
                b.draw(renderer);
            }
        }

        //draw ball
        ball.color = {241,90,34,255};
        ball.draw(renderer);

        SDL_RenderPresent(renderer);
    }

    void clean() {
        if(renderer) SDL_DestroyRenderer(renderer);
        if(window) SDL_DestroyWindow(window);
        SDL_Quit();
    }

    bool running() { return isRunning; }
};