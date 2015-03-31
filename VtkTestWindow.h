#pragma once

#include <QVTKWidget2.h>
#include <QVTKWidget.h>

class vtkRenderer;
class vtkDICOMImageReader;
class vtkMetaImageReader;
class vtkImageData;
class vtkVolume;
class vtkSmartVolumeMapper;

class VtkTestWindow : public QVTKWidget2
{
public:
    VtkTestWindow(QWidget* parent = nullptr);
	~VtkTestWindow();

    void addDicomData(vtkDICOMImageReader* dicomReader, vtkImageData* imageData);
    void addImage(const QString& imageFile);

private:
    vtkRenderer *ren1;
    vtkDICOMImageReader* m_dicomReader;
    vtkMetaImageReader* m_metaImageReader;
    vtkImageData* m_imageData;
    vtkVolume* m_volume;
    vtkSmartVolumeMapper* m_volumeMapper;
};

