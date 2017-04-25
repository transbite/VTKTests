#ifndef FOURWINDOWSTEST_H
#define FOURWINDOWSTEST_H

#include <QWidget>
#include "VtkTestsImage2DWindow.h"
#include <QTimer>
#include <QTime>
#include <QDebug>

namespace Ui {
class FourWindowsTest;
}

class FourWindowsTest : public QWidget
{
    Q_OBJECT
    enum IMAGE2DTYPE
    {
        AXIAL = 0,
        SAGITTAL = 1,
        CORONAL = 2,
        COUNT = 3
    };
public:
    explicit FourWindowsTest(QWidget *parent = 0);
    ~FourWindowsTest();
    VtkTestsImage2DWindow*     m_2DWindows[COUNT];
    //QVTKOpenGLWidget* m_volume;
    VtkTestWindow* m_volume;
signals:
    void fpsText(QString);
private slots:
    void onTimer();
protected:
    void showEvent(QShowEvent* );
    void hideEvent(QHideEvent* );
private:
    Ui::FourWindowsTest *ui;
    QTimer m_timer;
};

#endif // FOURWINDOWSTEST_H
