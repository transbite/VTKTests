#ifndef _VOLUME_PROPERTIES_CONTROLLER_H_
#define _VOLUME_PROPERTIES_CONTROLLER_H_

#include <QObject>

class vtkVolumeProperty;
class GraphWidget;
class vtkImageData;

class VolumePropertiesController : public QObject
{
    Q_OBJECT

public:
    VolumePropertiesController(QObject* parent = nullptr);
    ~VolumePropertiesController();

    void setData(vtkVolumeProperty* volumeProperty, GraphWidget* graphWidget, vtkImageData* imageData);
    GraphWidget* graphWidget();
    vtkImageData* imageData();

private slots:
    void onGraphColorChanged(int channel);

private:
    vtkVolumeProperty* m_volumeProperty;
    GraphWidget* m_graphWidget;
    vtkImageData* m_imageData;
};

#endif //_VOLUME_PROPERTIES_CONTROLLER_H_
