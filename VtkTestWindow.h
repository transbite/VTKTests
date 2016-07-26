#pragma once

#include <QVTKWidget2.h>
#include <QVTKWidget.h>
#include <QVTKOpenGLWidget.h>
#include <vtkSmartPointer.h>

#include <QTimer>

#define VtkTestWindowSuperClass QVTKOpenGLWidget

class VtkTestWindow : public VtkTestWindowSuperClass
{
    Q_OBJECT

public:
    VtkTestWindow(QWidget* parent = nullptr);
    ~VtkTestWindow();

    void startAnimation();

private slots:
    void onTimer();

private:
    vtkSmartPointer<vtkRenderer> m_renderer;
    QTimer m_timer;
};

