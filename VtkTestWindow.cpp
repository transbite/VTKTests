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

    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renWin = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    this->SetRenderWindow(renWin);
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
    startAnimation();
}


VtkTestWindow::~VtkTestWindow()
{

}

void VtkTestWindow::onTimer()
{
    //this->GetRenderWindow()->Render();
    //this->update();
    //this->renderVTK();
    this->GetRenderWindow()->Render();
}

void VtkTestWindow::startAnimation()
{
    m_timer.start(1000.0 / 60.0 );
}
