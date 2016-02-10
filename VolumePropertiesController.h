#ifndef _VOLUME_PROPERTIES_CONTROLLER_H_
#define _VOLUME_PROPERTIES_CONTROLLER_H_

#include <QObject>

class vtkVolumeProperty;
class GraphWidget;

class VolumePropertiesController : public QObject
{
    Q_OBJECT

public:
    VolumePropertiesController(QObject* parent = nullptr);
    ~VolumePropertiesController();

    void setData(vtkVolumeProperty* volumeProperty, GraphWidget* graphWidget);
    GraphWidget* graphWidget();

private slots:
    void onGraphColorChanged(int channel);

private:
    vtkVolumeProperty* m_volumeProperty;
    GraphWidget* m_graphWidget;
};

#endif //_VOLUME_PROPERTIES_CONTROLLER_H_
