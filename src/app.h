#pragma once
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include "opengl_win.h"
#include <memory>
#include "obj_loader.h"
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QListWidgetItem>
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
    void showObjectSettings(std::shared_ptr<Shape> shape);

    void addItemToLightList(int i, const std::string& light_type);
    void onLightSourceSelected(QListWidgetItem *light_source);
    void showPointLightSourceSettings(std::shared_ptr<Light> point_light_source);
    void showSpotLightSourceSettings(std::shared_ptr<Light> spot_light_source);
    void showDirectionalLightSourceSettings(std::shared_ptr<Light> directional_light_source);

    ~MainWindow();
public slots:
    void addLightButtonClicked();
    void addCubeButtonClicked();
    void addMeshButtonClicked();

    void addPointLightButtonClicked();
    void addSpotLightButtonClicked();
    void addDirectionalLightButtonClicked();

private:
    int i{0};
    Ui::MainWindow* ui;
    OpenGLWidget* openglWidget;
    QStringListModel* listModel;
};