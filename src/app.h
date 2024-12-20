#pragma once
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include "opengl_win.h"
#include <memory>
#include "obj_loader.h"

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
    void addLightButtonClicked();
    void addCubeButtonClicked();
    void addMeshButtonClicked();
    void changeProjectionButtonClicked();
    void startScene();
private:
    void displayFPS();

    std::shared_ptr<QTimer> fpsUpdateTimer;
    int i{0};
    Ui::MainWindow* ui;
    OpenGLWidget* openglWidget; // Ваш виджет
};