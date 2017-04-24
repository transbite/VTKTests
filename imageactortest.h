#ifndef IMAGEACTORTEST_H
#define IMAGEACTORTEST_H
#include "QVTKOpenGLWidget.h"
#include <QTimer>
#include <QTime>
#include <QDebug>

class ImageActorTest : public QVTKOpenGLWidget
{
    Q_OBJECT

public:
    ImageActorTest();
signals:
    void fpsText(QString);
private slots:
    void onTimer();
protected:
    void showEvent(QShowEvent* );
    void hideEvent(QHideEvent* );
private:
    QTimer m_timer;
};

#endif // IMAGEACTORTEST_H
