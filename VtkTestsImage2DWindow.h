#ifndef _VTK_TESTS_IMAGE_2D_WINDOW_H_
#define _VTK_TESTS_IMAGE_2D_WINDOW_H_

#include "ui_VtkTestsImage2DWindow.h"

#include <vtkSmartPointer.h>

class vtkImageViewer2;
class vtkImageData;

class VtkTestsImage2DWindow : public QWidget
{
Q_OBJECT

public:
    VtkTestsImage2DWindow(QWidget* parent = nullptr);
    ~VtkTestsImage2DWindow();

    void setInputData(vtkImageData* imageData, int type);

private:
    Ui::VtkTestsImage2DWindow   ui;

    vtkSmartPointer<vtkImageViewer2>    m_imageViewer;
};

#endif //_VTK_TESTS_IMAGE_2D_WINDOW_H_
