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
public:
    VtkTestWindow(QWidget* parent = nullptr);
	~VtkTestWindow();

    void addDicomData(vtkDICOMImageReader* dicomReader, vtkImageData* imageData);
    void addImage(const QString& imageFile);

private:
    vtkSmartPointer<vtkRenderer> ren1;
    vtkSmartPointer<vtkDICOMImageReader> m_dicomReader;
    vtkSmartPointer<vtkMetaImageReader> m_metaImageReader;
    vtkSmartPointer<vtkImageData> m_imageData;
    vtkSmartPointer<vtkVolume> m_volume;
    vtkSmartPointer<vtkSmartVolumeMapper> m_volumeMapper;
};

