#pragma once
#include <QMainWindow>
#include "opengl_win.h"
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
public slots:
    void addCubeButtonClicked();
private:
    int i{0};
    Ui::MainWindow* ui;
    OpenGLWidget* openglWidget; // Ваш виджет
};