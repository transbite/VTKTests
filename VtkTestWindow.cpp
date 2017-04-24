#include "VtkTestWindow.h"

#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkInteractorStyleTrackballCamera.h>
//#include <vtkOpenGLExtensionManager.h>
#include <vtkRendererCollection.h>

#include <QDebug>

vtkSmartPointer<vtkRenderer> ren1;

VtkTestWindow::VtkTestWindow(QWidget* parent)
    : VtkTestWindowSuperClass(parent)
{
    m_renderer = vtkSmartPointer<vtkRenderer>::New();
//    ren1 = m_renderer;
//    ren1->SetBackground( 0.1, 0.2, 0.4 );

    vtkRenderWindow* renWin = this->GetRenderWindow();
    renWin->AddRenderer(m_renderer);
    vtkRenderer* ren = renWin->GetRenderers()->GetFirstRenderer();
    ren->SetBackground( 0.1, 0.2, 0.4 );
    static bool ft = true;
    if(ft)
    {
        ren1 = ren;
        ft = false;
    }

    vtkRenderWindowInteractor *iren = renWin->GetInteractor();//vtkRenderWindowInteractor::New();
    //iren->SetRenderWindow(renWin);

//    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style_1 = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
//    vtkSmartPointer<vtkInteractorStyle> style_2 = vtkSmartPointer<vtkInteractorStyle>::New();

//    iren->SetInteractorStyle(style_1);

    this->setGeometry(this->pos().x(), this->pos().y(), 500, 500);
    ren1->ResetCamera();

    //test OpenGL extensions
//    vtkSmartPointer<vtkOpenGLExtensionManager> extManager = vtkSmartPointer<vtkOpenGLExtensionManager>::New();
//    extManager->SetRenderWindow(renWin);
//    if(extManager->ExtensionSupported("GL_ARB_compatibility") != 0)
//    {
//        extManager->LoadExtension("GL_ARB_compatibility");
//    }
//    else
//    {
//        qDebug() << "GL_ARB_compatibility cannot be loaded.";
//    }

    this->setAttribute(Qt::WA_DontCreateNativeAncestors);

    connect(&m_timer, &QTimer::timeout, this, &VtkTestWindow::onTimer);
    //startAnimation();
}


VtkTestWindow::~VtkTestWindow()
{

}

void VtkTestWindow::showEvent(QShowEvent* )
{
    m_timer.start(1000.0 / 60.0);
}

void VtkTestWindow::hideEvent(QHideEvent* )
{
    m_timer.stop();
}
void VtkTestWindow::onTimer()
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
        m_fpsText = fpsString;
        qDebug() << fpsString;
        emit fpsText(m_fpsText);
        fpsTimer.restart();
    }
}

void VtkTestWindow::startAnimation()
{
    m_timer.start(1000.0 / 60.0 );
}
