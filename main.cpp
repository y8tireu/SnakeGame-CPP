#include <QApplication>
#include "snakegame.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    SnakeGame window;
    window.setWindowTitle("Snake Game in Qt");
    window.resize(400, 400);  // Set a suitable size
    window.show();

    return app.exec();
}

