#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QScopedPointer>

namespace Ui {
class MainWindow;
}

class QDockWidget;
class VtkTests;
class VtkTestsImage2DWindow;
class VolumePropertiesController;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    friend class navicadVTKDicomPluginInterfaceTracker;

    enum IMAGE2DTYPE
    {
        AXIAL = 0,
        SAGITTAL = 1,
        CORONAL = 2,
        COUNT = 3
    };

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionLoadImage_triggered();
    void on_actionLoadDicom_triggered();
    void on_createDockWidget(QWidget* contentWidget);
    void on_removeDockWidget(const QString& windowTitle);
    void on_VolumePropertiesControllerCreated(VolumePropertiesController* controller);

private:
    Ui::MainWindow *ui;
    QScopedPointer<VtkTests> m_vtkTests;

    QMap<QString, QDockWidget*> m_docks;
    VtkTestsImage2DWindow*     m_2DWindows[COUNT];
};

#endif // MAINWINDOW_H
