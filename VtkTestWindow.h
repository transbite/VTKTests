#pragma once

#include <QVTKWidget2.h>
#include <QVTKWidget.h>
#include <vtkSmartPointer.h>

class vtkRenderer;
class vtkDICOMImageReader;
class vtkMetaImageReader;
class vtkImageData;
class vtkVolume;
class vtkSmartVolumeMapper;
class vtkVolumeMapper;

class VtkTestWindow : public QVTKWidget2
{
    Q_OBJECT

public:
    VtkTestWindow(QWidget* parent = nullptr);
    ~VtkTestWindow();

private:
    vtkSmartPointer<vtkRenderer> m_renderer;
};

