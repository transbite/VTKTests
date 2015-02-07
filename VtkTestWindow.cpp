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
	: QVTKWidget(parent)
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

	vtkRenderer *ren1= vtkRenderer::New();
	ren1->AddActor( coneActor );
	ren1->SetBackground( 0.1, 0.2, 0.4 );
	ren1->AddActor( coneActor2 );
	ren1->AddActor(axes);

	vtkRenderWindow* renWin = this->GetRenderWindow();
	renWin->AddRenderer(ren1);

	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	vtkInteractorStyleTrackballCamera *style =
		vtkInteractorStyleTrackballCamera::New();
// 	vtkInteractorStyle *style =
// 		vtkInteractorStyle::New();

 	iren->SetInteractorStyle(style);
	
	vtkBoxWidget *boxWidget = vtkBoxWidget::New();
	boxWidget->SetInteractor(iren);
	boxWidget->SetPlaceFactor(1.0);

	boxWidget->SetProp3D(coneActor);
	boxWidget->PlaceWidget();

	vtkMyCallback *callback = vtkMyCallback::New();
	boxWidget->AddObserver(vtkCommand::InteractionEvent, callback);

	boxWidget->On();

 	this->setGeometry(this->pos().x(), this->pos().y(), 500, 500);
	
	iren->Initialize();
	//iren->Start();
}


VtkTestWindow::~VtkTestWindow()
{

}
