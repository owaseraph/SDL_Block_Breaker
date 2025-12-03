# SDL3 Block Breaker

A modern C++ implementation of the classic Breakout arcade game, built using the **SDL3** library and **GLM** for vector mathematics.

## 🎮 Features

* **Physics-based Gameplay:** Advanced ball deflection logic where the bounce angle depends on the impact point on the paddle.

* **Dynamic Scaling:** Fully resizable window with aspect-ratio preservation (Letterboxing).

* **Game States:** Complete game flow including Start, Playing, Win, and Loss states.

* **Visual Feedback:** 
    * Paddle turns Cyan while playing.

    * Paddle turns Red on Game Over.

    * Paddle turns Green on Victory.

* Robust Collision: Precise Circle-to-Rectangle collision detection.


## 🛠️ Tech Stack

* **Language:** C++17

* **Graphics:** SDL3 (Preview)

* **Math: GLM** (OpenGL Mathematics)

* **Build System:** CMake

## 🚀 How to Build


### Prerequisites

* CMake

* C++ Compiler (MSVC, GCC, or Clang)

* SDL3 Library

* GLM Library

### Build Steps
```bash
mkdir build
cd build
cmake ..
cmake --build .
```