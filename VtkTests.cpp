#include "VtkTests.h"
#include "VolumePropertiesController.h"
#include "GraphWidget.h"

#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkCamera.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkProperty.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkBoxWidget.h>
#include <vtkTransform.h>
#include <vtkAxesActor.h>
#include <vtkCommand.h>
#include <vtkRenderer.h>
#include <vtkDICOMImageReader.h>
#include <vtkImageData.h>

#include <QMessageBox>
#include <QDebug>

class vtkMyCallback : public vtkCommand
{
public:
    static vtkMyCallback *New()
    { return new vtkMyCallback; }
    virtual void Execute(vtkObject *caller, unsigned long, void*)
    {
        vtkTransform *t = vtkTransform::New();
        vtkBoxWidget *widget = reinterpret_cast<vtkBoxWidget*>(caller);
        widget->GetTransform(t);
        widget->GetProp3D()->SetUserTransform(t);
        t->Delete();
    }
};

extern vtkSmartPointer<vtkRenderer> ren1;

void addCone()
{
    vtkAxesActor* axes = vtkAxesActor::New();
    axes->SetScale(3.0);

    vtkConeSource *cone = vtkConeSource::New();
    cone->SetHeight( 3.0 );
    cone->SetRadius( 1.0 );
    cone->SetResolution( 100 );

    vtkPolyDataMapper *coneMapper = vtkPolyDataMapper::New();
    coneMapper->SetInputConnection( cone->GetOutputPort() );

    vtkActor *coneActor = vtkActor::New();
    coneActor->SetMapper( coneMapper );
    coneActor->GetProperty()->SetColor(0.2, 0.63, 0.79);
    coneActor->GetProperty()->SetDiffuse(0.7);
    coneActor->GetProperty()->SetSpecular(0.4);
    coneActor->GetProperty()->SetSpecularPower(20);
    coneActor->GetProperty()->SetOpacity(0.45);

    vtkProperty *property = vtkProperty::New();
    property->SetColor(1.0, 0.3882, 0.2784);
    property->SetOpacity(0.5);
    property->SetDiffuse(0.7);
    property->SetSpecular(0.9);
    property->SetSpecularPower(50);
    vtkActor *coneActor2 = vtkActor::New();
    coneActor2->SetMapper(coneMapper);
    coneActor2->GetProperty()->SetColor(0.2, 0.63, 0.79);
    coneActor2->SetProperty(property);
    coneActor2->SetPosition(0, 2, 5);

    ren1->AddActor( coneActor );
    ren1->AddActor( coneActor2 );
    ren1->AddActor(axes);

    vtkBoxWidget *boxWidget = vtkBoxWidget::New();
    vtkRenderWindowInteractor* iren = ren1->GetRenderWindow()->GetInteractor();
    boxWidget->SetInteractor(iren);
    boxWidget->SetPlaceFactor(1.0);

    boxWidget->SetProp3D(coneActor);
    boxWidget->PlaceWidget();

    vtkMyCallback *callback = vtkMyCallback::New();
    boxWidget->AddObserver(vtkCommand::InteractionEvent, callback);

    boxWidget->On();
}

QSharedPointer<VolumePropertiesController> addDicomData(vtkDICOMImageReader* dicomReader, vtkImageData* imageData);
QSharedPointer<VolumePropertiesController> addDicomDirectory(const QString &dirName);
QSharedPointer<VolumePropertiesController> addImage(const QString &imageFile);

//VTK User's Guide Ch 6.2
#include <vtkQuadric.h>
#include <vtkSampleFunction.h>
#include <vtkExtractVOI.h>
#include <vtkContourFilter.h>
void testVtkExtractVOI()
{
    vtkSmartPointer<vtkQuadric> quadric = vtkSmartPointer<vtkQuadric>::New();
    quadric->SetCoefficients(0.5, 1, 0.2, 0, 0.1, 0, 0, 0.2, 0, 0);
    vtkSmartPointer<vtkSampleFunction> sample = vtkSmartPointer<vtkSampleFunction>::New();
    sample->SetSampleDimensions(128, 128, 128);
    sample->SetImplicitFunction(quadric);
    sample->ComputeNormalsOff();

    vtkSmartPointer<vtkExtractVOI> extract = vtkSmartPointer<vtkExtractVOI>::New();
    extract->SetInputConnection(sample->GetOutputPort());
    extract->SetVOI(0, 127, 0, 127, 64, 127);
    vtkSmartPointer<vtkContourFilter> contours = vtkSmartPointer<vtkContourFilter>::New();
    contours->SetInputConnection(extract->GetOutputPort());
//    contours->SetInputConnection(sample->GetOutputPort());
    contours->GenerateValues(10, 0.0, 1.2);
    vtkSmartPointer<vtkPolyDataMapper> contMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    contMapper->SetInputConnection(contours->GetOutputPort());
    contMapper->SetScalarRange(0.0, 1.2);
    vtkSmartPointer<vtkActor> contActor = vtkSmartPointer<vtkActor>::New();
    contActor->SetMapper(contMapper);

    ren1->AddActor(contActor);
}

VtkTests::VtkTests(QObject* parent)
    : QObject(parent)
{
    //addCone();
    testVtkExtractVOI();
}

VtkTests::~VtkTests()
{

}

void VtkTests::executeCommand(const Commands &command, const QString &arguments)
{
    switch(command)
    {
    case Commands::LOAD_DICOM:
        {
            QSharedPointer<VolumePropertiesController> volContr = addDicomDirectory(arguments);
            GraphWidget* graphWidget = volContr->graphWidget();
            QString windowTitle = graphWidget->windowTitle();
            m_volumeControllers.insert(windowTitle, volContr);
            emit volumePropertiesControllerCreated(volContr.data());
            emit createDockWidget(graphWidget);
        }
        break;
    case Commands::LOAD_IMAGE:
        {
            QSharedPointer<VolumePropertiesController> volContr = addImage(arguments);
            GraphWidget* graphWidget = volContr->graphWidget();
            QString windowTitle = graphWidget->windowTitle();
            m_volumeControllers.insert(windowTitle, volContr);
            emit volumePropertiesControllerCreated(volContr.data());
            emit createDockWidget(graphWidget);
        }
        break;

    default:
        break;
    }
}
