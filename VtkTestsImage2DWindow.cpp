#include "VtkTestsImage2DWindow.h"

#include <vtkImageViewer2.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkImageData.h>

VtkTestsImage2DWindow::VtkTestsImage2DWindow(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    m_imageViewer = vtkSmartPointer<vtkImageViewer2>::New();
    m_imageViewer->SetRenderWindow(ui.vtkWidget->GetRenderWindow());
    m_imageViewer->SetupInteractor(ui.vtkWidget->GetRenderWindow()->GetInteractor());

}

VtkTestsImage2DWindow::~VtkTestsImage2DWindow()
{

}

void VtkTestsImage2DWindow::setInputData(vtkImageData *imageData, int type)
{
    m_imageViewer->SetInputData(imageData);
    //axial
    if (type == 0) m_imageViewer->SetSliceOrientationToXY();
    //sagittal
    if(type == 1) m_imageViewer->SetSliceOrientationToYZ();
    //coronal
    if (type == 2) m_imageViewer->SetSliceOrientationToXZ();
}
