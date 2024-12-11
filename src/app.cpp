#include "app.h"
#include "./ui_mainwindow.h"
#include <QDebug>

namespace {
    const int WIDTH = 512;
    const int HEIGT = 512;
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) 
{
    ui->setupUi(this);
    openglWidget = new OpenGLWidget(ui->openGLWidget); // Используем контейнер из .ui
    setWindowTitle("Игры Разума");
    setGeometry(400, 200, 800, 600);
    qDebug() << openglWidget->size();
    openglWidget->resize(720, 720);
}

MainWindow::~MainWindow() {
    delete ui;
}
