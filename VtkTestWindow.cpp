#include "VtkTestWindow.h"

#include "vtkConeSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkCamera.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkProperty.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkBoxWidget.h"
#include "vtkTransform.h"
#include "vtkAxesActor.h"
#include "vtkCommand.h"

#include <vtkDICOMImageReader.h>
#include <vtkImageData.h>
#include <vtkVolume.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkImageResample.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkOpenGLExtensionManager.h>
#include <vtkOpenGLRenderWindow.h>
#include <vtkOpenGLRenderer.h>
#include <vtkOpenGLGPUVolumeRayCastMapper.h>
#include <vtkOpenGLVolumeTextureMapper3D.h>
#include <vtkFixedPointVolumeRayCastMapper.h>
#include <vtkMetaImageReader.h>
#include <vtkAlgorithmOutput.h>

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

VtkTestWindow::VtkTestWindow(QWidget* parent)
    : QVTKWidget2(parent)
{
//	vtkAxesActor* axes = vtkAxesActor::New();
//	axes->SetScale(3.0);

//	vtkConeSource *cone = vtkConeSource::New();
//	cone->SetHeight( 3.0 );
//	cone->SetRadius( 1.0 );
//	cone->SetResolution( 100 );

//	vtkPolyDataMapper *coneMapper = vtkPolyDataMapper::New();
//	coneMapper->SetInputConnection( cone->GetOutputPort() );

//	vtkActor *coneActor = vtkActor::New();
//	coneActor->SetMapper( coneMapper );
//	coneActor->GetProperty()->SetColor(0.2, 0.63, 0.79);
//	coneActor->GetProperty()->SetDiffuse(0.7);
//	coneActor->GetProperty()->SetSpecular(0.4);
//	coneActor->GetProperty()->SetSpecularPower(20);
//	coneActor->GetProperty()->SetOpacity(0.45);

//	vtkProperty *property = vtkProperty::New();
//	property->SetColor(1.0, 0.3882, 0.2784);
//	property->SetOpacity(0.5);
//	property->SetDiffuse(0.7);
//	property->SetSpecular(0.9);
//	property->SetSpecularPower(50);
//	vtkActor *coneActor2 = vtkActor::New();
//	coneActor2->SetMapper(coneMapper);
//	coneActor2->GetProperty()->SetColor(0.2, 0.63, 0.79);
//	coneActor2->SetProperty(property);
//	coneActor2->SetPosition(0, 2, 5);

    ren1= vtkSmartPointer<vtkRenderer>::New();
    ren1->SetBackground( 0.1, 0.2, 0.4 );
//  ren1->AddActor( coneActor );
//	ren1->AddActor( coneActor2 );
//	ren1->AddActor(axes);

    vtkRenderWindow* renWin = this->GetRenderWindow();
	renWin->AddRenderer(ren1);

    vtkRenderWindowInteractor *iren = renWin->GetInteractor();//vtkRenderWindowInteractor::New();
    //iren->SetRenderWindow(renWin);

    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style_1 = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    vtkSmartPointer<vtkInteractorStyle> style_2 = vtkSmartPointer<vtkInteractorStyle>::New();

    //iren->SetInteractorStyle(style_1);
	
//    vtkBoxWidget *boxWidget = vtkBoxWidget::New();
//    boxWidget->SetInteractor(iren);
//    boxWidget->SetPlaceFactor(1.0);

//    boxWidget->SetProp3D(coneActor);
//    boxWidget->PlaceWidget();

//    vtkMyCallback *callback = vtkMyCallback::New();
//    boxWidget->AddObserver(vtkCommand::InteractionEvent, callback);

//    boxWidget->On();

    this->setGeometry(this->pos().x(), this->pos().y(), 500, 500);
	
    //iren->Initialize();
	//iren->Start();

    m_volume = vtkSmartPointer<vtkVolume>::New();
    m_volumeMapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();

    m_metaImageReader = vtkSmartPointer<vtkMetaImageReader>::New();

    ren1->ResetCamera();
}


VtkTestWindow::~VtkTestWindow()
{

}

void VtkTestWindow::addDicomData(vtkDICOMImageReader* dicomReader, vtkImageData* imageData)
{
    ///test-remove
#if 0
    QVTKWidget2* wdg = new QVTKWidget2();
    //vtkRenderWindow *renWin = wdg->GetRenderWindow();
    vtkGenericOpenGLRenderWindow *renWin = wdg->GetRenderWindow();

//    vtkOpenGLExtensionManager* extManager = vtkOpenGLExtensionManager::New();
//    extManager->SetRenderWindow(renWin);
//    if(extManager->ExtensionSupported("GL_ARB_compatibility") != 0)
//    {
//        extManager->LoadExtension("GL_ARB_compatibility");
//    }
//    else
//    {
//        qDebug() << "GL_ARB_compatibility cannot be loaded.";
//    }

#endif //0
    //

    m_dicomReader = dicomReader;
    m_imageData = imageData;

    m_volumeMapper->SetInputConnection(m_dicomReader->GetOutputPort());

    // Create our transfer function
    vtkSmartPointer<vtkColorTransferFunction> colorFun = vtkSmartPointer<vtkColorTransferFunction>::New();
    vtkSmartPointer<vtkPiecewiseFunction> opacityFun = vtkSmartPointer<vtkPiecewiseFunction>::New();

    // Create the property and attach the transfer functions
    vtkSmartPointer<vtkVolumeProperty> property = vtkSmartPointer<vtkVolumeProperty>::New();
    property->SetIndependentComponents(true);
    property->SetColor( colorFun );
    property->SetScalarOpacity( opacityFun );
    property->SetInterpolationTypeToLinear();

    m_volume->SetProperty(property);
    m_volume->SetMapper(m_volumeMapper);

    //CT_Skin
    colorFun->AddRGBPoint( -3024, 0, 0, 0, 0.5, 0.0 );
    colorFun->AddRGBPoint( -1000, .62, .36, .18, 0.5, 0.0 );
    colorFun->AddRGBPoint( -500, .88, .60, .29, 0.33, 0.45 );
    colorFun->AddRGBPoint( 3071, .83, .66, 1, 0.5, 0.0 );

    opacityFun->AddPoint(-3024, 0, 0.5, 0.0 );
    opacityFun->AddPoint(-1000, 0, 0.5, 0.0 );
    opacityFun->AddPoint(-500, 1.0, 0.33, 0.45 );
    opacityFun->AddPoint(3071, 1.0, 0.5, 0.0);

    m_volumeMapper->SetBlendModeToComposite();
    property->ShadeOn();
    property->SetAmbient(0.1);
    property->SetDiffuse(0.9);
    property->SetSpecular(0.2);
    property->SetSpecularPower(10.0);
    property->SetScalarOpacityUnitDistance(0.8919);

    //data bounds
    double dataBounds[6];
    m_dicomReader->GetOutput()->GetBounds(dataBounds);

    //mapper bounds
    double mapperBounds[6];
    m_volumeMapper->GetBounds(mapperBounds);

    //volume bounds
    double bounds[6];
    m_volume->GetBounds(bounds);

    ren1->AddVolume(m_volume);
    this->makeCurrent();
    ren1->ResetCamera();
    this->update();
    //this->GetRenderWindow()->Render();
  }

void VtkTestWindow::addImage(const QString &imageFile)
{
    m_metaImageReader->SetFileName(imageFile.toStdString().c_str());
    m_metaImageReader->Update();

    m_volumeMapper->SetInputConnection(m_metaImageReader->GetOutputPort());

    // Create our transfer function
    vtkSmartPointer<vtkColorTransferFunction> colorFun = vtkSmartPointer<vtkColorTransferFunction>::New();
    vtkSmartPointer<vtkPiecewiseFunction> opacityFun = vtkSmartPointer<vtkPiecewiseFunction>::New();

    // Create the property and attach the transfer functions
    vtkSmartPointer<vtkVolumeProperty> property = vtkSmartPointer<vtkVolumeProperty>::New();
    property->SetIndependentComponents(true);
    property->SetColor( colorFun );
    property->SetScalarOpacity( opacityFun );
    property->SetInterpolationTypeToLinear();

    m_volume->SetProperty(property);
    m_volume->SetMapper(m_volumeMapper);

    //CT_Skin
	colorFun->AddRGBPoint( -3024, 0, 0, 0, 0.5, 0.0 );
	colorFun->AddRGBPoint( -155, .55, .25, .15, 0.5, .92 );
	colorFun->AddRGBPoint( 217, .88, .60, .29, 0.33, 0.45 );
	colorFun->AddRGBPoint( 420, 1, .94, .95, 0.5, 0.0 );
	colorFun->AddRGBPoint( 3071, .83, .66, 1, 0.5, 0.0 );

	opacityFun->AddPoint(-3024, 0, 0.5, 0.0 );
	opacityFun->AddPoint(-155, 0, 0.5, 0.92 );
	opacityFun->AddPoint(217, .68, 0.33, 0.45 );
	opacityFun->AddPoint(420,.83, 0.5, 0.0);
	opacityFun->AddPoint(3071, .80, 0.5, 0.0);

    m_volumeMapper->SetBlendModeToComposite();
    property->ShadeOn();
    property->SetAmbient(0.1);
    property->SetDiffuse(0.9);
    property->SetSpecular(0.2);
    property->SetSpecularPower(10.0);
    property->SetScalarOpacityUnitDistance(0.8919);

    //data bounds
    double dataBounds[6];
    m_metaImageReader->GetOutput()->GetBounds(dataBounds);

    //mapper bounds
    double mapperBounds[6];
    m_volumeMapper->GetBounds(mapperBounds);

    //volume bounds
    double bounds[6];
    m_volume->GetBounds(bounds);

    ren1->AddVolume(m_volume);
    ren1->ResetCamera();

    this->GetRenderWindow()->Render();
}
