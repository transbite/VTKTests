#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "VtkTests.h"
#include "VtkTestsImage2DWindow.h"
#include "VolumePropertiesController.h"

#include <QDir>
#include <QApplication>
#include <QPluginLoader>
#include <QDockWidget>
#include <QDebug>
#include <QFileDialog>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkBoxWidget.h>
#include <vtkTransform.h>
#include <vtkAxesActor.h>
#include <vtkCommand.h>
#include <vtkRenderer.h>
#include <vtkDICOMImageReader.h>
#include <vtkImageData.h>
#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkInteractorStyleImage.h>
#include <QDebug>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->mainToolBar->addWidget(ui->label);
    ui->mainToolBar->addWidget(ui->comboBox);
    m_fourWindows = new FourWindowsTest(parent);
    m_imageActor = new ImageActorTest();
    m_imageSclicer = new ImageSlicerTest();
    m_controller = new VolumePropertiesController();
    /*m_2DWindows[AXIAL] = ui->vtkAxial;
    m_2DWindows[SAGITTAL] = ui->vtkSagittal;
    m_2DWindows[CORONAL] = ui->vtkCoronal;*/
    m_vtkTests.reset(new VtkTests());
    ui->comboBox->addItem("Choose an option");
    ui->comboBox->addItem("Four windows");
    ui->comboBox->addItem("One window - Image Actor");
    ui->comboBox->addItem("One window - Image Slice");

    bool b = connect(ui->actionQuit, &QAction::triggered, qApp, &QApplication::quit);
    Q_ASSERT(b);

    b = connect(m_vtkTests.data(), &VtkTests::createDockWidget, this, &MainWindow::on_createDockWidget);
    Q_ASSERT(b);
    b = connect(m_vtkTests.data(), &VtkTests::removeDockWidget, this, &MainWindow::on_removeDockWidget);
    Q_ASSERT(b);
    b = connect(m_vtkTests.data(), &VtkTests::volumePropertiesControllerCreated, this, &MainWindow::on_VolumePropertiesControllerCreated);
    Q_ASSERT(b);
    b = connect(ui->comboBox, &QComboBox::currentTextChanged, this, &MainWindow::on_ComboBoxChanged);
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
        m_vtkTests->executeCommand(VtkTests::Commands::LOAD_IMAGE, fileName);
    }
}

void MainWindow::on_actionLoadDicom_triggered()
{
    QString dirName = QFileDialog::getExistingDirectory(nullptr, tr("Open DICOM directory"), QDir::currentPath());
    if(dirName.isEmpty())
    {
        return;
    }

    m_vtkTests->executeCommand(VtkTests::Commands::LOAD_DICOM, dirName);
    m_dirName = dirName;
}

void MainWindow::on_createDockWidget(QWidget *contentWidget)
{
    QString windowTitle = contentWidget->windowTitle();
    QDockWidget* dock = new QDockWidget(this);
    dock->setWindowTitle(windowTitle);
    contentWidget->setParent(dock);
    dock->setMinimumWidth(350);
    dock->setFloating(false);
    dock->setWidget(contentWidget);
    this->addDockWidget(Qt::LeftDockWidgetArea, dock);
    QAction* dockToggleAction = dock->toggleViewAction();
    ui->menuWindow->addAction(dockToggleAction);

    m_docks.insert(windowTitle, dock);
}

void MainWindow::on_removeDockWidget(const QString &windowTitle)
{
    QDockWidget* dock = m_docks[windowTitle];
    dock->deleteLater();
}

void MainWindow::on_VolumePropertiesControllerCreated(VolumePropertiesController *controller)
{

   m_controller = controller;

}

void MainWindow::on_ComboBoxChanged()
{

  QString method = ui->comboBox->currentText();
  if(m_controller!= NULL)
  {
    if(method == "Four windows")
    {
        displayFourWindows(m_controller);
    }

    else if(method == "One window - Image Actor")
    {
        displayOneWindow1(m_controller);
    }

    else if (method == "One window - Image Slice")
    {
        displayOneWindow2(m_controller);
    }
  }
}
void MainWindow::displayFourWindows(VolumePropertiesController *controller){

  if(controller->imageData() != NULL)
  {
        m_fourWindows = new FourWindowsTest();
        if(this->centralWidget()&& this->centralWidget()!= m_fourWindows){
            this->centralWidget()->deleteLater();
        }

        this->setCentralWidget(m_fourWindows);
        for(int i = 0; i < COUNT; ++i)
         {
            m_fourWindows->m_2DWindows[i]->setInputData(controller->imageData(), i);
         }

        vtkRenderWindow* renWin = m_fourWindows->m_volume->GetRenderWindow();
        vtkSmartPointer<vtkRenderer> rendererVOL = vtkSmartPointer<vtkRenderer>::New();

        vtkVolume* volume = controller->volumeData();
        rendererVOL->AddVolume(volume);
        rendererVOL->ResetCamera();
        renWin->AddRenderer(rendererVOL);
       // VtkTestWindow *vtkTestWindow = m_fourWindows->m_volume ;
        connect(m_fourWindows, &FourWindowsTest::fpsText, this, &MainWindow::on_fpsText);
 }
}
void MainWindow::displayOneWindow1(VolumePropertiesController *controller){
   static double viewport[3][4] = {
          { 0.0, 0.5, 0.5, 1 },
          { 0.5, 0.5, 1.0, 1.0 },
          { 0.0, 0.0, 0.5, 0.5 },

        };
   if(controller->imageData() != NULL)
   {
        m_imageActor = new ImageActorTest();
        vtkRenderWindow *renWin = m_imageActor->GetRenderWindow();
        if(this->centralWidget()&& this->centralWidget() != m_imageActor){
            this->centralWidget()->deleteLater();
        }

        this->setCentralWidget(m_imageActor);
        double range[2];
        for(int i = 0; i < COUNT; ++i)
         {
            //image Actor
            vtkImageData* imageData = controller->imageData();
            imageData->GetScalarRange(range);
            vtkSmartPointer<vtkImageActor> imageActor = vtkSmartPointer<vtkImageActor>::New();
            imageActor->GetMapper()->SliceFacesCameraOn();
            imageActor->GetMapper()->SliceAtFocalPointOn();
            imageActor->GetMapper()->SetInputData(imageData);
            //
           /* vtkSmartPointer<vtkImageSlice> image = vtkSmartPointer<vtkImageSlice>::New();
            image->SetMapper(imageActor->GetMapper());
            image->GetProperty()->SetColorWindow(range[1] - range[0]);
            image->GetProperty()->SetColorLevel(0.5*(range[0] + range[1]));
            image->GetProperty()->SetInterpolationTypeToNearest();
           */
            //

            vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
            renderer->SetViewport(viewport[i]);
            //renderer->AddViewProp(image);
            renderer->AddActor(imageActor);
            renWin->AddRenderer(renderer);
            vtkVolume* volumeTest = controller->volumeData();
            double *bounds = volumeTest->GetBounds();

            double point[3];
            point[0] = 0.5*(bounds[0] + bounds[1]);
            point[1] = 0.5*(bounds[2] + bounds[3]);
            point[2] = 0.5*(bounds[4] + bounds[5]);
            double maxdim = 0.0;
            for (int j = 0; j < 3; j++)
            {
              double s = 0.5*(bounds[2*j+1] - bounds[2*j]);
              maxdim = (s > maxdim ? s : maxdim);
            }
            qDebug() << "volume bounds  " << maxdim;
            vtkCamera *camera = renderer->GetActiveCamera();
            camera->SetFocalPoint(point);
            point[i % 3] -= 500.0;
            camera->SetPosition(point);
            /*if ((i % 3) == 2)
            {
              camera->SetViewUp(0.0, -1.0, 0.0);
            }
            else
            {
              camera->SetViewUp(0.0, 0.0, +1.0);
            }*/

            if( i == 0 )

            {
                //axial
                camera->SetViewUp(0.0, 0.0, +1.0);
            }
            else if ( i == 1)
            {
               //sagittal slice
                camera->SetViewUp(0.0, 0.0, +1.0);
            }
            else if ( i == 2)
            {
               //coronal slice
               camera->SetViewUp(0.0, -1.0, 0.0);
            }
            camera->ParallelProjectionOn();
            camera->SetParallelScale(maxdim);
        }

        vtkSmartPointer<vtkRenderer> rendererVOL = vtkSmartPointer<vtkRenderer>::New();
        rendererVOL->SetViewport(0.5, 0.0, 1.0, 0.5);
        vtkVolume* volume = controller->volumeData();
        rendererVOL->AddVolume(volume);
        rendererVOL->ResetCamera();
        renWin->AddRenderer(rendererVOL);
        connect(m_imageActor, &ImageActorTest::fpsText, this, &MainWindow::on_fpsText);

        vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
        vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
        style->SetInteractionModeToImage3D();
       // iren->SetRenderWindow(renWin);
      //  iren->SetInteractorStyle(style);
   }

}

void MainWindow::displayOneWindow2(VolumePropertiesController *controller){

    static double viewport[3][4] = {
          { 0.0, 0.5, 0.5, 1 },
          { 0.5, 0.5, 1.0, 1.0 },
          { 0.0, 0.0, 0.5, 0.5 },

        };
    if(controller->imageData() != NULL)
    {
        m_imageSclicer = new ImageSlicerTest();
        vtkRenderWindow* renWin2 = m_imageSclicer->GetRenderWindow();

       if(this->centralWidget()&& this->centralWidget() != m_imageSclicer){
            this->centralWidget()->deleteLater();
        }
        this->setCentralWidget(m_imageSclicer);

        std::string filesDir = controller->dicomDir().toStdString();
        vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
        reader->SetDirectoryName(filesDir.c_str());
        reader->Update();
        qDebug() << "Testing"  << reader->GetPatientName() << reader->GetStudyID();
        vtkAlgorithmOutput* portToDisplay = reader->GetOutputPort();
        double range[2];
        int extent[6];
        reader->Update();

        reader->GetOutput()->GetScalarRange(range);
        reader->GetOutput()->GetExtent(extent);

        // check if image is 2D
        bool imageIs3D = (extent[5] > extent[4]);

        for (int i = 2*(imageIs3D == 0); i < 3; i++)
        {
          vtkSmartPointer<vtkImageResliceMapper> imageMapper =
            vtkSmartPointer<vtkImageResliceMapper>::New();
          if (i < 3)
          {
            imageMapper->SetInputConnection(portToDisplay);
          }
          imageMapper->SliceFacesCameraOn();
          imageMapper->SliceAtFocalPointOn();
          imageMapper->ResampleToScreenPixelsOn();

          vtkSmartPointer<vtkImageSlice> image =
            vtkSmartPointer<vtkImageSlice>::New();
          image->SetMapper(imageMapper);
          image->GetProperty()->SetColorWindow(range[1] - range[0]);
          image->GetProperty()->SetColorLevel(0.5*(range[0] + range[1]));
          image->GetProperty()->SetInterpolationTypeToNearest();

          vtkSmartPointer<vtkRenderer> renderer2 = vtkSmartPointer<vtkRenderer>::New();
          renderer2->AddViewProp(image);
          renderer2->SetBackground(0.0, 0.0, 0.0);
          if (imageIs3D)
          {
            renderer2->SetViewport(viewport[i]);
          }

          renWin2->AddRenderer(renderer2);

          // use center point to set camera
          double *bounds = imageMapper->GetBounds();
          double point[3];
          point[0] = 0.5*(bounds[0] + bounds[1]);
          point[1] = 0.5*(bounds[2] + bounds[3]);
          point[2] = 0.5*(bounds[4] + bounds[5]);

          double maxdim = 0.0;
          for (int j = 0; j < 3; j++)
          {
            double s = 0.5*(bounds[2*j+1] - bounds[2*j]);
            maxdim = (s > maxdim ? s : maxdim);
          }

          vtkCamera *camera = renderer2->GetActiveCamera();
          camera->SetFocalPoint(point);
          point[i % 3] -= 500.0;
          camera->SetPosition(point);
          if ((i % 3) == 2)
          {
            camera->SetViewUp(0.0, -1.0, 0.0);
          }
          else
          {
            camera->SetViewUp(0.0, 0.0, +1.0);
          }
          camera->ParallelProjectionOn();
          camera->SetParallelScale(maxdim);
        }

        vtkSmartPointer<vtkRenderer> rendererVOL2 = vtkSmartPointer<vtkRenderer>::New();
        rendererVOL2->SetViewport(0.5, 0.0, 1.0, 0.5);
        vtkVolume* volume2 = controller->volumeData();
        rendererVOL2->AddVolume(volume2);
        rendererVOL2->ResetCamera();
        renWin2->AddRenderer(rendererVOL2);
        connect(m_imageSclicer, &ImageSlicerTest::fpsText, this, &MainWindow::on_fpsText);

    }
}

void MainWindow::on_fpsText(QString fpsString)
{
    this->statusBar()->showMessage(fpsString);

}
