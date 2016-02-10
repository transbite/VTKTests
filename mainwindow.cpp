#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "VtkTests.h"

#include <QDir>
#include <QApplication>
#include <QPluginLoader>
#include <QDockWidget>
#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_vtkTests.reset(new VtkTests());
    bool b = connect(ui->actionQuit, &QAction::triggered, qApp, &QApplication::quit);
    Q_ASSERT(b);

    b = connect(m_vtkTests.data(), &VtkTests::createDockWidget, this, &MainWindow::on_createDockWidget);
    Q_ASSERT(b);
    b = connect(m_vtkTests.data(), &VtkTests::removeDockWidget, this, &MainWindow::on_removeDockWidget);
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
