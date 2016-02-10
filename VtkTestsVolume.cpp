#include <vtkSmartPointer.h>
#include <vtkDICOMImageReader.h>
#include <vtkImageData.h>
#include <vtkVolume.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkRenderer.h>
#include <vtkMetaImageReader.h>
#include <vtkInformation.h>

#include <QMessageBox>
#include <QDebug>

extern vtkSmartPointer<vtkRenderer> ren1;

void addDicomData(vtkDICOMImageReader* dicomReader, vtkImageData* imageData)
{
    vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
    volumeMapper->SetInputConnection(dicomReader->GetOutputPort());

    // Create our transfer function
    vtkSmartPointer<vtkColorTransferFunction> colorFun = vtkSmartPointer<vtkColorTransferFunction>::New();
    vtkSmartPointer<vtkPiecewiseFunction> opacityFun = vtkSmartPointer<vtkPiecewiseFunction>::New();

    // Create the property and attach the transfer functions
    vtkSmartPointer<vtkVolumeProperty> property = vtkSmartPointer<vtkVolumeProperty>::New();
    property->SetIndependentComponents(true);
    property->SetColor( colorFun );
    property->SetScalarOpacity( opacityFun );
    property->SetInterpolationTypeToLinear();

    vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();

    volume->SetProperty(property);
    volume->SetMapper(volumeMapper);

    //CT_Skin
    colorFun->AddRGBPoint( -3024, 0, 0, 0, 0.5, 0.0 );
    colorFun->AddRGBPoint( -1000, .62, .36, .18, 0.5, 0.0 );
    colorFun->AddRGBPoint( -500, .88, .60, .29, 0.33, 0.45 );
    colorFun->AddRGBPoint( 3071, .83, .66, 1, 0.5, 0.0 );

    opacityFun->AddPoint(-3024, 0, 0.5, 0.0 );
    opacityFun->AddPoint(-1000, 0, 0.5, 0.0 );
    opacityFun->AddPoint(-500, 1.0, 0.33, 0.45 );
    opacityFun->AddPoint(3071, 1.0, 0.5, 0.0);

    volumeMapper->SetBlendModeToComposite();
    property->ShadeOn();
    property->SetAmbient(0.1);
    property->SetDiffuse(0.9);
    property->SetSpecular(0.2);
    property->SetSpecularPower(10.0);
    property->SetScalarOpacityUnitDistance(0.8919);

    //data bounds
    double dataBounds[6];
    dicomReader->GetOutput()->GetBounds(dataBounds);

    //mapper bounds
    double mapperBounds[6];
    volumeMapper->GetBounds(mapperBounds);

    //volume bounds
    double bounds[6];
    volume->GetBounds(bounds);

    ren1->AddVolume(volume);
    ren1->ResetCamera();
    ren1->Render();
}

void addDicomDirectory(const QString &dirName)
{
    vtkDICOMImageReader* dicomReader = vtkDICOMImageReader::New();
    dicomReader->SetDirectoryName(dirName.toStdString().c_str());
    dicomReader->Update();

    vtkImageData* dicomData = dicomReader->GetOutput();

    //bounds
    double bounds[6];
    dicomData->GetBounds(bounds);

    int dim[3] = {0, 0, 0};
    if(dicomData != nullptr)
    {
        dicomData->GetDimensions(dim);
    }
    if(dim[0] < 2 || dim[1] < 2 || dim[2] < 2)
    {
        QMessageBox::warning(nullptr, "Dicom loader", "Error loading dicom data!", QMessageBox::Ok);
        return;
    }
    QMessageBox::information(nullptr, "Dicom loader", "Dicom data loaded.", QMessageBox::Ok);

    QString patientName = dicomReader->GetPatientName();
    QString studyId = dicomReader->GetStudyID();
    qDebug() << "Patient name: " << patientName << "; Study id: " << studyId;
    vtkInformation* dicomInformation = dicomReader->GetInformation();
    vtkIndent indent(2);
    std::cout << "Dicom information: " << endl;
    dicomInformation->PrintHeader(std::cout, indent);
    dicomInformation->PrintKeys(std::cout, indent);

    addDicomData(dicomReader, dicomData);
}

void addImage(const QString &imageFile)
{
    vtkSmartPointer<vtkMetaImageReader> metaImageReader = vtkSmartPointer<vtkMetaImageReader>::New();

    metaImageReader->SetFileName(imageFile.toStdString().c_str());
    metaImageReader->Update();

    vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
    volumeMapper->SetInputConnection(metaImageReader->GetOutputPort());

    // Create our transfer function
    vtkSmartPointer<vtkColorTransferFunction> colorFun = vtkSmartPointer<vtkColorTransferFunction>::New();
    vtkSmartPointer<vtkPiecewiseFunction> opacityFun = vtkSmartPointer<vtkPiecewiseFunction>::New();

    // Create the property and attach the transfer functions
    vtkSmartPointer<vtkVolumeProperty> property = vtkSmartPointer<vtkVolumeProperty>::New();
    property->SetIndependentComponents(true);
    property->SetColor( colorFun );
    property->SetScalarOpacity( opacityFun );
    property->SetInterpolationTypeToLinear();

    vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();

    volume->SetProperty(property);
    volume->SetMapper(volumeMapper);

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

    volumeMapper->SetBlendModeToComposite();
    property->ShadeOn();
    property->SetAmbient(0.1);
    property->SetDiffuse(0.9);
    property->SetSpecular(0.2);
    property->SetSpecularPower(10.0);
    property->SetScalarOpacityUnitDistance(0.8919);

    //data bounds
    double dataBounds[6];
    metaImageReader->GetOutput()->GetBounds(dataBounds);

    //mapper bounds
    double mapperBounds[6];
    volumeMapper->GetBounds(mapperBounds);

    //volume bounds
    double bounds[6];
    volume->GetBounds(bounds);

    ren1->AddVolume(volume);
    ren1->ResetCamera();
    ren1->Render();
}
