# Snake Game in C++ Using Qt

This document demonstrates how to implement a simple **Snake** game using **Qt** in C++. It includes:

1. A basic project structure (using `qmake`).
2. Source code for:
   - A `.pro` file
   - `main.cpp`
   - `snakegame.h`
   - `snakegame.cpp`
3. Instructions on how to build and run the application.

---

## Table of Contents

1. [Project Overview](#project-overview)
2. [Project Structure](#project-structure)
3. [SnakeGame.pro](#snakegamepro)
4. [main.cpp](#maincpp)
5. [snakegame.h](#snakegameh)
6. [snakegame.cpp](#snakegamecpp)
7. [Build and Run Instructions](#build-and-run-instructions)
    - [Using Qt Creator](#using-qt-creator)
    - [Using qmake Command Line](#using-qmake-command-line)
8. [Gameplay Overview](#gameplay-overview)
9. [Possible Extensions](#possible-extensions)

---

## Project Overview

In this simple Snake game:

- The **snake** is represented by a list of grid positions (`QVector<QPoint>`).
- The **food** is a single grid cell placed randomly on the board.
- A `QTimer` drives the game logic by calling `updateGame()` periodically.
- We handle painting in `paintEvent()` to draw the snake segments and the food.
- Arrow keys move the snake; if the snake runs into a wall or itself, the game ends.
- Press `Space` to restart after **Game Over**.

---

## Project Structure

A minimal Qt (qmake) project might look like:

```
SnakeGame
├── SnakeGame.pro
├── main.cpp
├── snakegame.h
└── snakegame.cpp
```

- **SnakeGame.pro**: Qt project file, used by `qmake`.
- **main.cpp**: Application entry point, sets up the main window.
- **snakegame.h/cpp**: Contains the `SnakeGame` class, including all game logic and rendering.

---

## SnakeGame.pro

Create a file named **SnakeGame.pro** with the following content:

```prolog
TEMPLATE = app
CONFIG += c++11
QT += core gui widgets

SOURCES += \
    main.cpp \
    snakegame.cpp

HEADERS += \
    snakegame.h
```

### Explanation

- `TEMPLATE = app` means we’re building an application (executable).
- `CONFIG += c++11` enables C++11 features.
- `QT += core gui widgets` ensures we can use Qt’s Core, GUI, and Widgets modules (including `QApplication`, `QWidget`, etc.).

---

## main.cpp

```cpp
#include <QApplication>
#include "snakegame.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    SnakeGame window;
    window.setWindowTitle("Snake Game in Qt");
    window.resize(400, 400);  // Adjust as needed
    window.show();

    return app.exec();
}
```

### Explanation

- We create a `QApplication` object to manage application-wide resources.
- An instance of `SnakeGame` (our main widget) is created, sized, and shown.
- Finally, we enter the Qt event loop with `app.exec()`.

---

## snakegame.h

```cpp
#ifndef SNAKEGAME_H
#define SNAKEGAME_H

#include <QWidget>
#include <QTimer>
#include <QKeyEvent>
#include <QVector>
#include <QPoint>

class SnakeGame : public QWidget
{
    Q_OBJECT

public:
    explicit SnakeGame(QWidget* parent = nullptr);
    ~SnakeGame();

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void updateGame();

private:
    void initGame();
    void moveSnake();
    void checkCollision();
    void spawnFood();

    // Board size constants
    static const int BOARD_WIDTH = 20;      // Cells horizontally
    static const int BOARD_HEIGHT = 20;     // Cells vertically
    static const int CELL_SIZE = 20;        // Pixel size per cell
    static const int DELAY = 100;           // Game update interval in ms

    // Snake data
    QVector<QPoint> snake;                 // List of positions (head is first)
    QPoint food;                           // Single food location
    enum Direction { UP, DOWN, LEFT, RIGHT };
    Direction dir;                         // Current moving direction
    bool inGame;                           // Is the game running?

    QTimer timer;                          // Drives the game updates
};

#endif // SNAKEGAME_H
```

### Explanation

- `BOARD_WIDTH`, `BOARD_HEIGHT`, and `CELL_SIZE` define the size of the game grid.
- `Direction` tracks the snake’s movement direction.
- `snake` is a list of points on the grid (the first element is the head).
- `food` is a point on the grid representing the current food position.
- `inGame` indicates if the player is still alive.
- `QTimer` triggers `updateGame()` at a set interval to advance game logic.

---

## snakegame.cpp

```cpp
#include "snakegame.h"
#include <QPainter>
#include <QRandomGenerator>

SnakeGame::SnakeGame(QWidget *parent)
    : QWidget(parent), dir(RIGHT), inGame(true)
{
    // Fix the window size based on the board and cell size
    setFixedSize(BOARD_WIDTH * CELL_SIZE, BOARD_HEIGHT * CELL_SIZE);

    initGame();

    connect(&timer, &QTimer::timeout, this, &SnakeGame::updateGame);
    timer.start(DELAY);
}

SnakeGame::~SnakeGame()
{
    // Destructor (unused here)
}

void SnakeGame::initGame()
{
    // Clear any existing snake data
    snake.clear();

    // Start the snake in the center
    snake.push_back(QPoint(BOARD_WIDTH / 2, BOARD_HEIGHT / 2));
    snake.push_back(QPoint((BOARD_WIDTH / 2) - 1, BOARD_HEIGHT / 2));
    snake.push_back(QPoint((BOARD_WIDTH / 2) - 2, BOARD_HEIGHT / 2));

    // Initial direction
    dir = RIGHT;
    inGame = true;

    // Spawn the first piece of food
    spawnFood();
}

void SnakeGame::spawnFood()
{
    // Generate random positions until we find one that is not on the snake
    int x = QRandomGenerator::global()->bounded(BOARD_WIDTH);
    int y = QRandomGenerator::global()->bounded(BOARD_HEIGHT);
    food = QPoint(x, y);

    while (snake.contains(food)) {
        x = QRandomGenerator::global()->bounded(BOARD_WIDTH);
        y = QRandomGenerator::global()->bounded(BOARD_HEIGHT);
        food = QPoint(x, y);
    }
}

void SnakeGame::updateGame()
{
    if (inGame) {
        moveSnake();
        checkCollision();
    }
    // Trigger repaint
    update();
}

void SnakeGame::moveSnake()
{
    // Head is at snake[0]
    QPoint head = snake.first();

    // Move head based on current direction
    switch (dir) {
    case LEFT:  head.setX(head.x() - 1); break;
    case RIGHT: head.setX(head.x() + 1); break;
    case UP:    head.setY(head.y() - 1); break;
    case DOWN:  head.setY(head.y() + 1); break;
    }

    // Insert new head
    snake.push_front(head);

    // Check if we've eaten the food
    if (head == food) {
        // Snake grows by not removing the tail
        spawnFood();
    } else {
        // Remove the last segment (tail) to keep length consistent
        snake.pop_back();
    }
}

void SnakeGame::checkCollision()
{
    QPoint head = snake.first();

    // Check boundary collisions
    if (head.x() < 0 || head.x() >= BOARD_WIDTH ||
        head.y() < 0 || head.y() >= BOARD_HEIGHT) {
        inGame = false;
    }

    // Check collision with itself
    for (int i = 1; i < snake.size(); ++i) {
        if (snake[i] == head) {
            inGame = false;
            break;
        }
    }

    // If game is over, stop the timer
    if (!inGame) {
        timer.stop();
    }
}

void SnakeGame::paintEvent(QPaintEvent* /*event*/)
{
    QPainter painter(this);

    if (inGame) {
        // Draw food
        painter.setBrush(Qt::red);
        painter.drawRect(food.x() * CELL_SIZE,
                         food.y() * CELL_SIZE,
                         CELL_SIZE, CELL_SIZE);

        // Draw snake
        painter.setBrush(Qt::green);
        for (const QPoint &pt : snake) {
            painter.drawRect(pt.x() * CELL_SIZE,
                             pt.y() * CELL_SIZE,
                             CELL_SIZE, CELL_SIZE);
        }
    } else {
        // Game Over text
        painter.setPen(Qt::black);
        painter.setFont(QFont("Arial", 20, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter, "Game Over!\nPress SPACE to Restart");
    }
}

void SnakeGame::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Left:
        if (dir != RIGHT) dir = LEFT;
        break;
    case Qt::Key_Right:
        if (dir != LEFT) dir = RIGHT;
        break;
    case Qt::Key_Up:
        if (dir != DOWN) dir = UP;
        break;
    case Qt::Key_Down:
        if (dir != UP) dir = DOWN;
        break;
    case Qt::Key_Space:
        // Restart when pressing space, if game is over
        if (!inGame) {
            initGame();
            timer.start(DELAY);
        }
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}
```

### Explanation

1. **initGame()**: Places the snake in the center, sets direction, and spawns food.
2. **spawnFood()**: Picks a random cell and checks if it’s free.
3. **updateGame()**: Called by the `QTimer`, moves the snake and checks collisions.
4. **moveSnake()**: Inserts a new head in front and removes tail unless food was eaten.
5. **checkCollision()**: Ends the game if the snake hits a wall or itself.
6. **paintEvent()**: Draws the snake segments and the food; if `inGame` is false, shows “Game Over!”.
7. **keyPressEvent()**: Handles arrow keys and restarts the game with Space.

---

## Build and Run Instructions

### Using Qt Creator

1. Install [Qt](https://www.qt.io) and open **Qt Creator**.
2. **File → Open File or Project...** → select `SnakeGame.pro`.
3. Choose your Kit, configure the project, then click **Build** or **Run**.

### Using qmake Command Line

1. Make sure you have the Qt development tools installed and `qmake` in your PATH.
2. Open a terminal in the `SnakeGame` folder (where `SnakeGame.pro` is located).
3. Run:
   ```bash
   qmake
   make
   ```
   On Windows with MinGW, you might do `qmake` then `mingw32-make`.
4. Run the produced executable (e.g., `./SnakeGame` on Linux or `SnakeGame.exe` on Windows).

---

## Gameplay Overview

- **Movement**: Use the **Left**, **Right**, **Up**, and **Down** arrow keys to move.
- **Eating Food**: The snake grows longer when it eats the food.
- **Collision**: Hitting the walls or your own snake body ends the game.
- **Restart**: Press **Space** to restart after **Game Over**.

---

## Possible Extensions

- **Score Tracking**: Display a score that increments each time you eat food.
- **Difficulty Levels**: Increase speed (`DELAY`) as the snake grows or over time.
- **Custom Graphics**: Replace the green and red rectangles with custom images for snake/food.
- **Pause Functionality**: Bind a key to pause/resume the game.
- **Levels or Obstacles**: Introduce walls or obstacles to navigate around.
