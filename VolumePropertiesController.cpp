#include "VolumePropertiesController.h"
#include "GraphWidget.h"

#include <vtkVolumeProperty.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkImageData.h>

#include <vector>

extern vtkSmartPointer<vtkRenderer> ren1;

VolumePropertiesController::VolumePropertiesController(QObject *parent)
    : QObject(parent)
    , m_volumeProperty(nullptr)
    , m_graphWidget(nullptr)
    , m_imageData(nullptr)
{

}

VolumePropertiesController::~VolumePropertiesController()
{

}

void VolumePropertiesController::setData(vtkVolumeProperty *volumeProperty, GraphWidget *graphWidget, vtkImageData *imageData)
{
    m_volumeProperty = volumeProperty;
    m_graphWidget = graphWidget;

    bool b = connect((GraphWidget*)graphWidget, &GraphWidget::signalColorChanged,
                     this, &VolumePropertiesController::onGraphColorChanged);
    Q_ASSERT(b);
    onGraphColorChanged(0);
    onGraphColorChanged(1);
    onGraphColorChanged(2);
    m_graphWidget->Points(3);

    m_imageData = imageData;
}

GraphWidget* VolumePropertiesController::graphWidget()
{
    return m_graphWidget;
}

vtkImageData* VolumePropertiesController::imageData()
{
    return m_imageData;
}

void VolumePropertiesController::onGraphColorChanged(int channel)
{
    double min = m_graphWidget->Xmin();
    double max = m_graphWidget->Xmax();
    std::vector<double> points = m_graphWidget->Points(channel);
    int sz = points.size();
    const double dx = (max - min) / (sz - 1);
    if(channel == 0)
    {
        vtkPiecewiseFunction* opacity = m_volumeProperty->GetScalarOpacity();
        opacity->RemoveAllPoints();
        for(int i = 0; i < sz; ++i)
        {
            double x = min + i * dx;
            double y = points[i];
            opacity->AddPoint(x, y);
        }
    }
    else
    {
        vtkColorTransferFunction* color = m_volumeProperty->GetRGBTransferFunction();
        for(int i = 0; i < sz; ++i)
        {
            double x = min + i * dx;
            double red = m_graphWidget->Points(1)[i];
            double green = m_graphWidget->Points(2)[i];
            double blue = m_graphWidget->Points(3)[i];
            color->AddRGBPoint(x, red, green, blue);
        }
    }

    //ren1->Render();
}
