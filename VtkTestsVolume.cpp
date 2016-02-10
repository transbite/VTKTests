#include "VolumePropertiesController.h"
#include "GraphWidget.h"

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

#include <QSharedPointer>
#include <QMessageBox>
#include <QDebug>

extern vtkSmartPointer<vtkRenderer> ren1;
static int volCount = 0;

QSharedPointer<VolumePropertiesController> addDicomData(vtkDICOMImageReader* dicomReader, vtkImageData* imageData)
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

    volumeMapper->SetBlendModeToComposite();
    property->ShadeOn();
    property->SetAmbient(0.1);
    property->SetDiffuse(0.9);
    property->SetSpecular(0.2);
    property->SetSpecularPower(10.0);
    property->SetScalarOpacityUnitDistance(0.8919);

    QSharedPointer<VolumePropertiesController> volContr(new VolumePropertiesController());
    GraphWidget* graphWidget = new GraphWidget();
    graphWidget->setWindowTitle(QString("Volume %1").arg(++volCount));
    double* scalarRange = imageData->GetScalarRange();
    graphWidget->InitData(scalarRange[0], scalarRange[1]);
    volContr->setData(property, graphWidget);

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

    return volContr;
}

QSharedPointer<VolumePropertiesController> addDicomDirectory(const QString &dirName)
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
        return QSharedPointer<VolumePropertiesController>(nullptr);
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

    return addDicomData(dicomReader, dicomData);
}

QSharedPointer<VolumePropertiesController> addImage(const QString &imageFile)
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

    QSharedPointer<VolumePropertiesController> volContr(new VolumePropertiesController());
    GraphWidget* graphWidget = new GraphWidget();
    graphWidget->setWindowTitle(QString("Volume %1").arg(++volCount));
    double* scalarRange = metaImageReader->GetOutput()->GetScalarRange();
    graphWidget->InitData(scalarRange[0], scalarRange[1]);
    volContr->setData(property, graphWidget);

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

    return volContr;
}
