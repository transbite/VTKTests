#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QApplication>
#include <QPluginLoader>
#include <QDockWidget>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

#include <vtkDICOMImageReader.h>
#include <vtkImageData.h>
#include <vtkInformation.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_vtkTestWindow = ui->widget;

    bool b = connect(ui->actionQuit, &QAction::triggered, qApp, &QApplication::quit);
    Q_ASSERT(b);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionLoadImage_triggered()
{
    // Open a Qt dialog for to select a `.mhd` file.
    QString fileName = QFileDialog::getOpenFileName(
        this, tr("Open Meta Image"), ".", tr("Image Files (*.mhd)"));

    // `fileName` will be `NULL` if the user clicked "Cancel".
    if (fileName != NULL)
    {
        m_vtkTestWindow->addImage(fileName);
    }
}

void MainWindow::on_actionLoadDicom_triggered()
{
    QString dirName = QFileDialog::getExistingDirectory(nullptr, tr("Open DICOM directory"), QDir::currentPath());
    if(dirName.isEmpty())
    {
        return;
    }

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
        QMessageBox::warning(nullptr, tr("Dicom loader"), tr("Error loading dicom data!"), QMessageBox::Ok);
        return;
    }
    QMessageBox::information(nullptr, tr("Dicom loader"), tr("Dicom data loaded."), QMessageBox::Ok);

    QString patientName = dicomReader->GetPatientName();
    QString studyId = dicomReader->GetStudyID();
    qDebug() << "Patient name: " << patientName << "; Study id: " << studyId;
    vtkInformation* dicomInformation = dicomReader->GetInformation();
    vtkIndent indent(2);
    std::cout << "Dicom information: " << endl;
    dicomInformation->PrintHeader(std::cout, indent);
    dicomInformation->PrintKeys(std::cout, indent);

    m_vtkTestWindow->addDicomData(dicomReader, dicomData);
}
