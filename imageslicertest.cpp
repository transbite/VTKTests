#include "imageslicertest.h"

ImageSlicerTest::ImageSlicerTest()
{
    connect(&m_timer, &QTimer::timeout, this, &ImageSlicerTest::onTimer);
}
void ImageSlicerTest::showEvent(QShowEvent* )
{
    m_timer.start(1000.0 / 60.0);
}

void ImageSlicerTest::hideEvent(QHideEvent* )
{
    m_timer.stop();
}
void ImageSlicerTest::onTimer()
{
    //this->GetRenderWindow()->Render();
    /*this->update();
    qDebug() << "Timer";*/

    static int count = 0;
    static const int count_max = 60;
    static QTime fpsTimer;
    static bool ft = true;
    if(ft)
    {
        fpsTimer.start();
        ft = false;
    }

    this->update();

    count += 1;

    if(count == count_max)
    {
        count = 0;
        double elapsedTimeSeconds = fpsTimer.elapsed() * 0.001;
        double fps = count_max / elapsedTimeSeconds;
        QString fpsString = QString::number(fps, 'f', 2) + " FPS";
        //m_fpsText->SetInput(fpsText.toStdString().c_str());
        qDebug() << fpsString;
        emit fpsText(fpsString);
        fpsTimer.restart();
    }
}
