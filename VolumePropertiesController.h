#ifndef _VOLUME_PROPERTIES_CONTROLLER_H_
#define _VOLUME_PROPERTIES_CONTROLLER_H_

#include <QObject>
#include <vtkDICOMImageReader.h>
#include <vtkVolume.h>

class vtkVolumeProperty;
class GraphWidget;
class vtkImageData;

class VolumePropertiesController : public QObject
{
    Q_OBJECT

public:
    VolumePropertiesController(QObject* parent = nullptr);
    ~VolumePropertiesController();

    void setData(vtkVolumeProperty* volumeProperty, GraphWidget* graphWidget, vtkImageData* imageData, vtkVolume* volumeData, QString dicomDir);
    void setData(vtkVolumeProperty* volumeProperty, GraphWidget* graphWidget, vtkImageData* imageData);
    GraphWidget* graphWidget();
    vtkImageData* imageData();
    vtkVolume* volumeData();
    QString dicomDir();


private slots:
    void onGraphColorChanged(int channel);

private:
    vtkVolumeProperty* m_volumeProperty;
    GraphWidget* m_graphWidget;
    vtkImageData* m_imageData;
    vtkVolume* m_volumeData;
    QString m_dicomDir;

};

#endif //_VOLUME_PROPERTIES_CONTROLLER_H_
