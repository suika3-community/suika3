#include <QApplication>
#include <QMainWindow>
#include "qtgamewidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow window;
    GameWidget* gameView = GameWidget::createSingleton("");

    window.setCentralWidget(gameView);
    window.resize(800, 600);
    window.show();

    gameView->start();

    return app.exec();
}
