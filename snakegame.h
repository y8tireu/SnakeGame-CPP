#ifndef SNAKEGAME_H
#define SNAKEGAME_H

#include <QWidget>
#include <QTimer>
#include <QKeyEvent>

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

    // Constants for board size and speed
    static const int BOARD_WIDTH = 20;     // number of cells horizontally
    static const int BOARD_HEIGHT = 20;    // number of cells vertically
    static const int CELL_SIZE = 20;       // pixel size of each cell
    static const int DELAY = 100;          // timer interval in ms

    // Snake data
    QVector<QPoint> snake; // stores segments as (x, y) positions on the grid
    QPoint food;
    enum Direction { UP, DOWN, LEFT, RIGHT };
    Direction dir;
    bool inGame;

    QTimer timer;
};

#endif // SNAKEGAME_H

