#pragma once

#include <QVTKWidget2.h>
#include <QVTKWidget.h>
#include <QVTKOpenGLWidget.h>
#include <vtkSmartPointer.h>

#include <QTimer>
#include <QTime>

#define VtkTestWindowSuperClass QVTKOpenGLWidget

class VtkTestWindow : public VtkTestWindowSuperClass
{
    Q_OBJECT

public:
    VtkTestWindow(QWidget* parent = nullptr);
    ~VtkTestWindow();
    void startAnimation();
    QString m_fpsText;
signals:
    void fpsText(QString);
private slots:
    void onTimer();
protected:
    void showEvent(QShowEvent* );
    void hideEvent(QHideEvent* );

private:
    vtkSmartPointer<vtkRenderer> m_renderer;
    QTimer m_timer;
};

