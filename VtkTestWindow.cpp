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
#include <vtkMetaImageReader.h>

#include <QDebug>

void addVolume(vtkRenderWindow* renWin, vtkRenderer* renderer, vtkAlgorithm* reader, vtkImageData* input, int blendType = 3)
{
  bool independentComponents = true;
  double reductionFactor = 1.0;
  double opacityWindow = 4096;
  double opacityLevel = 2048;
  double frameRate = 10.0;
  int clip = 0;

 // Connect it all. Note that funny arithematic on the
  // SetDesiredUpdateRate - the vtkRenderWindow divides it
  // allocated time across all renderers, and the renderer
  // divides it time across all props. If clip is
  // true then there are two props
//  vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
//  iren->SetRenderWindow(renWin);
  //iren->SetDesiredUpdateRate(frameRate / (1+clip) );

  //iren->GetInteractorStyle()->SetDefaultRenderer(renderer);

  // Verify that we actually have a volume
  int dim[3];
  input->GetDimensions(dim);
  if ( dim[0] < 2 ||
       dim[1] < 2 ||
       dim[2] < 2 )
    {
    std::cout << "Error loading data!" << endl;
    return;
    }

  vtkImageResample *resample = vtkImageResample::New();
  if ( reductionFactor < 1.0 )
    {
    resample->SetInputConnection( reader->GetOutputPort() );
    resample->SetAxisMagnificationFactor(0, reductionFactor);
    resample->SetAxisMagnificationFactor(1, reductionFactor);
    resample->SetAxisMagnificationFactor(2, reductionFactor);
    }

  // Create our volume and mapper
  vtkVolume *volume = vtkVolume::New();
  vtkSmartVolumeMapper *mapper = vtkSmartVolumeMapper::New();
  //vtkOpenGLGPUVolumeRayCastMapper* mapper = vtkOpenGLGPUVolumeRayCastMapper::New();
  //vtkOpenGLVolumeTextureMapper3D* mapper = vtkOpenGLVolumeTextureMapper3D::New();

  if ( reductionFactor < 1.0 )
    {
    mapper->SetInputConnection( resample->GetOutputPort() );
    }
  else
    {
    mapper->SetInputConnection( reader->GetOutputPort() );
    }


  // Set the sample distance on the ray to be 1/2 the average spacing
  double spacing[3];
  if ( reductionFactor < 1.0 )
    {
    resample->GetOutput()->GetSpacing(spacing);
    }
  else
    {
    input->GetSpacing(spacing);
    }

//  mapper->SetSampleDistance( (spacing[0]+spacing[1]+spacing[2])/6.0 );
//  mapper->SetMaximumImageSampleDistance(10.0);


  // Create our transfer function
  vtkColorTransferFunction *colorFun = vtkColorTransferFunction::New();
  vtkPiecewiseFunction *opacityFun = vtkPiecewiseFunction::New();

  // Create the property and attach the transfer functions
  vtkVolumeProperty *property = vtkVolumeProperty::New();
  property->SetIndependentComponents(independentComponents);
  property->SetColor( colorFun );
  property->SetScalarOpacity( opacityFun );
  property->SetInterpolationTypeToLinear();

  // connect up the volume to the property and the mapper
  volume->SetProperty( property );
  volume->SetMapper( mapper );

  // Depending on the blend type selected as a command line option,
  // adjust the transfer function
  switch ( blendType )
    {
    // MIP
    // Create an opacity ramp from the window and level values.
    // Color is white. Blending is MIP.
    case 0:
      colorFun->AddRGBSegment(0.0, 1.0, 1.0, 1.0, 255.0, 1.0, 1.0, 1.0 );
      opacityFun->AddSegment( opacityLevel - 0.5*opacityWindow, 0.0,
                              opacityLevel + 0.5*opacityWindow, 1.0 );
      mapper->SetBlendModeToMaximumIntensity();
      break;

    // CompositeRamp
    // Create a ramp from the window and level values. Use compositing
    // without shading. Color is a ramp from black to white.
    case 1:
      colorFun->AddRGBSegment(opacityLevel - 0.5*opacityWindow, 0.0, 0.0, 0.0,
                              opacityLevel + 0.5*opacityWindow, 1.0, 1.0, 1.0 );
      opacityFun->AddSegment( opacityLevel - 0.5*opacityWindow, 0.0,
                              opacityLevel + 0.5*opacityWindow, 1.0 );
      mapper->SetBlendModeToComposite();
      property->ShadeOff();
      break;

    // CompositeShadeRamp
    // Create a ramp from the window and level values. Use compositing
    // with shading. Color is white.
    case 2:
      colorFun->AddRGBSegment(0.0, 1.0, 1.0, 1.0, 255.0, 1.0, 1.0, 1.0 );
      opacityFun->AddSegment( opacityLevel - 0.5*opacityWindow, 0.0,
                              opacityLevel + 0.5*opacityWindow, 1.0 );
      mapper->SetBlendModeToComposite();
      property->ShadeOn();
      break;

    // CT_Skin
    // Use compositing and functions set to highlight skin in CT data
    // Not for use on RGB data
    case 3:
      colorFun->AddRGBPoint( -3024, 0, 0, 0, 0.5, 0.0 );
      colorFun->AddRGBPoint( -1000, .62, .36, .18, 0.5, 0.0 );
      colorFun->AddRGBPoint( -500, .88, .60, .29, 0.33, 0.45 );
      colorFun->AddRGBPoint( 3071, .83, .66, 1, 0.5, 0.0 );

      opacityFun->AddPoint(-3024, 0, 0.5, 0.0 );
      opacityFun->AddPoint(-1000, 0, 0.5, 0.0 );
      opacityFun->AddPoint(-500, 1.0, 0.33, 0.45 );
      opacityFun->AddPoint(3071, 1.0, 0.5, 0.0);

      mapper->SetBlendModeToComposite();
      property->ShadeOn();
      property->SetAmbient(0.1);
      property->SetDiffuse(0.9);
      property->SetSpecular(0.2);
      property->SetSpecularPower(10.0);
      property->SetScalarOpacityUnitDistance(0.8919);
      break;

    // CT_Bone
    // Use compositing and functions set to highlight bone in CT data
    // Not for use on RGB data
    case 4:
      colorFun->AddRGBPoint( -3024, 0, 0, 0, 0.5, 0.0 );
      colorFun->AddRGBPoint( -16, 0.73, 0.25, 0.30, 0.49, .61 );
      colorFun->AddRGBPoint( 641, .90, .82, .56, .5, 0.0 );
      colorFun->AddRGBPoint( 3071, 1, 1, 1, .5, 0.0 );

      opacityFun->AddPoint(-3024, 0, 0.5, 0.0 );
      opacityFun->AddPoint(-16, 0, .49, .61 );
      opacityFun->AddPoint(641, .72, .5, 0.0 );
      opacityFun->AddPoint(3071, .71, 0.5, 0.0);

      mapper->SetBlendModeToComposite();
      property->ShadeOn();
      property->SetAmbient(0.1);
      property->SetDiffuse(0.9);
      property->SetSpecular(0.2);
      property->SetSpecularPower(10.0);
      property->SetScalarOpacityUnitDistance(0.8919);
      break;

    // CT_Muscle
    // Use compositing and functions set to highlight muscle in CT data
    // Not for use on RGB data
    case 5:
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

      mapper->SetBlendModeToComposite();
      property->ShadeOn();
      property->SetAmbient(0.1);
      property->SetDiffuse(0.9);
      property->SetSpecular(0.2);
      property->SetSpecularPower(10.0);
      property->SetScalarOpacityUnitDistance(0.8919);
      break;

    // RGB_Composite
    // Use compositing and functions set to highlight red/green/blue regions
    // in RGB data. Not for use on single component data
    case 6:
      opacityFun->AddPoint(0, 0.0);
      opacityFun->AddPoint(5.0, 0.0);
      opacityFun->AddPoint(30.0, 0.05);
      opacityFun->AddPoint(31.0, 0.0);
      opacityFun->AddPoint(90.0, 0.0);
      opacityFun->AddPoint(100.0, 0.3);
      opacityFun->AddPoint(110.0, 0.0);
      opacityFun->AddPoint(190.0, 0.0);
      opacityFun->AddPoint(200.0, 0.4);
      opacityFun->AddPoint(210.0, 0.0);
      opacityFun->AddPoint(245.0, 0.0);
      opacityFun->AddPoint(255.0, 0.5);

      mapper->SetBlendModeToComposite();
      property->ShadeOff();
      property->SetScalarOpacityUnitDistance(1.0);
      break;
    default:
       vtkGenericWarningMacro("Unknown blend type.");
       break;
    }

  // Set the default window size
  //renWin->SetSize(600,600);
  renWin->Render();

   // Add the volume to the scene
  renderer->AddVolume( volume );

  renderer->ResetCamera();

  // interact with data
  renWin->Render();

   // iren->Start();
}

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

    ren1= vtkRenderer::New();
    ren1->SetBackground( 0.1, 0.2, 0.4 );
//  ren1->AddActor( coneActor );
//	ren1->AddActor( coneActor2 );
//	ren1->AddActor(axes);

    /*vtkRenderWindow**/ vtkGenericOpenGLRenderWindow* renWin = this->GetRenderWindow();
	renWin->AddRenderer(ren1);

//    vtkRenderWindowInteractor *iren = renWin->GetInteractor();//vtkRenderWindowInteractor::New();
    //iren->SetRenderWindow(renWin);

    //vtkInteractorStyleTrackballCamera *style = vtkInteractorStyleTrackballCamera::New();
    //vtkInteractorStyle *style = vtkInteractorStyle::New();

    //iren->SetInteractorStyle(style);
	
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

    m_volume = vtkVolume::New();
    m_volumeMapper = vtkSmartVolumeMapper::New();

    m_metaImageReader = vtkMetaImageReader::New();
}


VtkTestWindow::~VtkTestWindow()
{

}

void VtkTestWindow::addDicomData(vtkDICOMImageReader* dicomReader, vtkImageData* imageData)
{
    ///test-remove
#if 1
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

    //vtkRenderer *renderer = vtkOpenGLRenderer::New();
    vtkRenderer *renderer = vtkRenderer::New();
    renderer->SetBackground(0.5, 0.9, 0.7);

    renWin->AddRenderer(renderer);

    wdg->SetRenderWindow(renWin);
    wdg->resize(600, 600);
    wdg->show();
    addVolume(renWin, renderer, dicomReader, imageData);
    return;
#endif //0
    //

    m_dicomReader = dicomReader;
    m_imageData = imageData;

    //test-remove
    vtkRenderWindowInteractor* irn = this->GetRenderWindow()->GetInteractor();
    irn->SetDesiredUpdateRate(30.0);
    //

    //m_volume = vtkVolume::New();
    //m_volumeMapper = vtkSmartVolumeMapper::New();

    m_volumeMapper->SetInputConnection(m_dicomReader->GetOutputPort());

    // Create our transfer function
    vtkColorTransferFunction* colorFun = vtkColorTransferFunction::New();
    vtkPiecewiseFunction* opacityFun = vtkPiecewiseFunction::New();

    // Create the property and attach the transfer functions
    vtkVolumeProperty *property = vtkVolumeProperty::New();
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

    ren1->AddVolume(m_volume);
    ren1->ResetCamera();
//    vtkConeSource *cone = vtkConeSource::New();
//    cone->SetHeight( 3.0 );
//    cone->SetRadius( 1.0 );
//    cone->SetResolution( 100 );
//    vtkPolyDataMapper *coneMapper = vtkPolyDataMapper::New();
//    coneMapper->SetInputConnection( cone->GetOutputPort() );
//    vtkActor *coneActor = vtkActor::New();
//    coneActor->SetMapper( coneMapper );
//    coneActor->GetProperty()->SetColor(0.2, 0.63, 0.79);
//    coneActor->GetProperty()->SetDiffuse(0.7);
//    coneActor->GetProperty()->SetSpecular(0.4);
//    coneActor->GetProperty()->SetSpecularPower(20);
//    coneActor->GetProperty()->SetOpacity(0.45);
//    ren1->AddActor(coneActor);

    this->GetRenderWindow()->Render();
  }

void VtkTestWindow::addImage(const QString &imageFile)
{
    m_metaImageReader->SetFileName(imageFile.toStdString().c_str());
    m_metaImageReader->Update();

    m_volumeMapper->SetInputConnection(m_metaImageReader->GetOutputPort());

    // Create our transfer function
    vtkColorTransferFunction* colorFun = vtkColorTransferFunction::New();
    vtkPiecewiseFunction* opacityFun = vtkPiecewiseFunction::New();

    // Create the property and attach the transfer functions
    vtkVolumeProperty *property = vtkVolumeProperty::New();
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

    ren1->AddVolume(m_volume);
    ren1->ResetCamera();

    this->GetRenderWindow()->Render();
}
