#pragma once

#include <QVTKWidget2.h>
#include <QVTKWidget.h>

class vtkRenderer;
class vtkDICOMImageReader;
class vtkImageData;
class vtkVolume;
class vtkSmartVolumeMapper;

class VtkTestWindow : public QVTKWidget
{
public:
    VtkTestWindow(QWidget* parent = nullptr);
	~VtkTestWindow();

    void addDicomData(vtkDICOMImageReader* dicomReader, vtkImageData* imageData);

private:
    vtkRenderer *ren1;
    vtkDICOMImageReader* m_dicomReader;
    vtkImageData* m_imageData;
    vtkVolume* m_volume;
    vtkSmartVolumeMapper* m_volumeMapper;
};

