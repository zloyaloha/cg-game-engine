#include "app.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.resize(800, 600);
    w.show();
    return app.exec();
}