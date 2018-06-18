#include "VtkTestsImage2DWindow.h"

#include <vtkImageViewer2.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkImageData.h>

VtkTestsImage2DWindow::VtkTestsImage2DWindow(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    m_imageViewer = vtkSmartPointer<vtkImageViewer2>::New();
    vtkRenderWindow *renWin = ui.vtkWidget->GetRenderWindow();
    vtkRenderWindowInteractor *renWinInteractor =  renWin->GetInteractor();
    m_imageViewer->SetRenderWindow(renWin);
    m_imageViewer->SetupInteractor(renWinInteractor);
}

VtkTestsImage2DWindow::~VtkTestsImage2DWindow()
{

}

void VtkTestsImage2DWindow::setInputData(vtkImageData *imageData)
{
    m_imageViewer->SetInputData(imageData);
}
