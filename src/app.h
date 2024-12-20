#pragma once
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include "opengl_win.h"
#include <memory>
#include "obj_loader.h"
#include <QLabel>
#include <QLineEdit>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QVariant>
#include <QFormLayout>
#include <QGroupBox>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    void addItemToList(int i, const std::string& type);
    void onObjectSelected(QListWidgetItem *item);
    void showObjectSettings(std::shared_ptr<Shape> shape, int groupId);
    ~MainWindow();
private:
    std::string getPenultimateWord(const QString& qstringPath);
public slots:
    void addLightButtonClicked();
    void addCubeButtonClicked();
    void addMeshButtonClicked();
    void changeProjectionButtonClicked();
    void restorePosition();
    void startScene();
private:
    void displayFPS();

    std::shared_ptr<QTimer> fpsUpdateTimer;
    int i{0};
    Ui::MainWindow* ui;
    OpenGLWidget* openglWidget;
    QStringListModel* listModel;
    std::unordered_map<int, std::vector<std::shared_ptr<Mesh>>> objectGroups;
};