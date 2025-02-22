#include "snakegame.h"
#include <QPainter>
#include <QRandomGenerator>

SnakeGame::SnakeGame(QWidget *parent)
    : QWidget(parent), dir(RIGHT), inGame(true)
{
    setFixedSize(BOARD_WIDTH * CELL_SIZE, BOARD_HEIGHT * CELL_SIZE);
    initGame();

    connect(&timer, &QTimer::timeout, this, &SnakeGame::updateGame);
    timer.start(DELAY);  // Start game update timer
}

SnakeGame::~SnakeGame()
{
    // Clean up if needed
}

void SnakeGame::initGame()
{
    // Initialize snake in the center
    snake.clear();
    snake.push_back(QPoint(BOARD_WIDTH/2, BOARD_HEIGHT/2));
    snake.push_back(QPoint(BOARD_WIDTH/2 - 1, BOARD_HEIGHT/2));
    snake.push_back(QPoint(BOARD_WIDTH/2 - 2, BOARD_HEIGHT/2));

    dir = RIGHT;
    inGame = true;

    spawnFood();
}

void SnakeGame::spawnFood()
{
    // Randomize a position for the food
    int x = QRandomGenerator::global()->bounded(BOARD_WIDTH);
    int y = QRandomGenerator::global()->bounded(BOARD_HEIGHT);
    food = QPoint(x, y);

    // Make sure the food is not on the snake
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
    update();  // trigger a repaint
}

void SnakeGame::moveSnake()
{
    // Create a new head based on current direction
    QPoint head = snake.first();
    switch (dir) {
        case LEFT:  head.setX(head.x() - 1); break;
        case RIGHT: head.setX(head.x() + 1); break;
        case UP:    head.setY(head.y() - 1); break;
        case DOWN:  head.setY(head.y() + 1); break;
    }

    // Insert new head at the front
    snake.push_front(head);

    // Check if we've eaten food
    if (head == food) {
        // Keep the tail to grow the snake and spawn new food
        spawnFood();
    } else {
        // Remove the tail
        snake.pop_back();
    }
}

void SnakeGame::checkCollision()
{
    QPoint head = snake.first();

    // Check collision with walls
    if (head.x() < 0 || head.x() >= BOARD_WIDTH ||
        head.y() < 0 || head.y() >= BOARD_HEIGHT) {
        inGame = false;
    }

    // Check collision with self (snake body)
    for (int i = 1; i < snake.size(); ++i) {
        if (snake[i] == head) {
            inGame = false;
            break;
        }
    }

    if (!inGame) {
        timer.stop();
    }
}

void SnakeGame::paintEvent(QPaintEvent * /*event*/)
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
        // Game Over screen
        painter.setPen(Qt::black);
        painter.setFont(QFont("Arial", 20, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter, "Game Over!");
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
        // Restart the game with Space if it's over
        if (!inGame) {
            initGame();
            timer.start(DELAY);
        }
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}

