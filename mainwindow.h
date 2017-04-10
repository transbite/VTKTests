#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QScopedPointer>
#include <vtkRenderWindow.h>
#include <vtkImageActor.h>
#include <vtkImageMapper3D.h>
#include <vtkRenderer.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include "QVTKOpenGLWidget.h"
#include <vtkSmartPointer.h>
#include "vtkGenericOpenGLRenderWindow.h"
#include "vtkImageReslice.h"
#include "vtkImageResliceMapper.h"
#include "vtkImageProperty.h"
#include "vtkImageSlice.h"
#include "vtkImageReader2.h"
#include "fourwindowstest.h"
#include "imageactortest.h"
#include "imageslicertest.h"

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
    void displayFourWindows(VolumePropertiesController* controller);
    void displayOneWindow1(VolumePropertiesController* controller);
    void displayOneWindow2(VolumePropertiesController* controller);

private slots:
    void on_actionLoadImage_triggered();
    void on_actionLoadDicom_triggered();
    void on_createDockWidget(QWidget* contentWidget);
    void on_removeDockWidget(const QString& windowTitle);
    void on_VolumePropertiesControllerCreated(VolumePropertiesController* controller);
    void on_ComboBoxChanged();

private:
    Ui::MainWindow *ui;
    QScopedPointer<VtkTests> m_vtkTests;

    QMap<QString, QDockWidget*> m_docks;
    VtkTestsImage2DWindow*     m_2DWindows[COUNT];
    FourWindowsTest*  m_fourWindows;
    ImageActorTest* m_imageActor;
    ImageSlicerTest* m_imageSclicer;
    QString m_dirName;
    VolumePropertiesController* m_controller;
};

#endif // MAINWINDOW_H
