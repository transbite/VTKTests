#ifndef IMAGESLICERTEST_H
#define IMAGESLICERTEST_H
#include "QVTKOpenGLWidget.h"
#include <QTimer>
#include <QTime>
#include <QDebug>
class ImageSlicerTest : public QVTKOpenGLWidget
{
     Q_OBJECT
public:
    ImageSlicerTest();
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

#endif // IMAGESLICERTEST_H
