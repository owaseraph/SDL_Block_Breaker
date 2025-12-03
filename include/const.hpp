#ifndef CONST_HPP
#define CONST_HPP

#pragma once

//screen
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

//ball
const float BALL_RADIUS = 10.0f;
const float BALL_SPEED = 7.5f;

//paddle
const float PADDLE_WIDTH = 100.0f;
const float PADDLE_HEIGHT = 25.0f;
const float PADDLE_SPEED = 8.0f;
const float PADDLE_Y = SCREEN_HEIGHT - 50.0f;

//brick
const int BRICK_ROWS = 3;
const int BRICK_COLS = 17;
const float BRICK_WIDTH = 70.0f;
const float BRICK_HEIGHT = 30.0f;
const float BRICK_OFFSET_X = 35.0f;
const float BRICK_OFFSET_Y = 50.0f;
const float BRICK_PADDING = 5.0f;

#endif // CONST_HPP
