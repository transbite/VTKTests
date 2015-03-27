#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Interfaces.h"

#include "ro.navicad.VTKDicom/navicadVTKDicomPluginInterface.h"
#include "ro.navicad.CTKDicom/navicadCTKDicomPluginInterface.h"

#include <QDir>
#include <QApplication>
#include <QPluginLoader>
#include <QDockWidget>
#include <QDebug>

#include <ctkPluginFrameworkLauncher.h>
#include <ctkPluginFrameworkFactory.h>
#include <ctkPluginFramework.h>
#include <ctkPluginException.h>
#include <ctkPluginContext.h>
#include <ctkServiceTrackerCustomizer.h>

class navicadVTKDicomPluginInterfaceTracker : public ctkServiceTrackerCustomizer<navicadVTKDicomPluginInterface*>
{
public:
    navicadVTKDicomPluginInterfaceTracker(MainWindow* w);
    ~navicadVTKDicomPluginInterfaceTracker();
private:
    navicadVTKDicomPluginInterface* addingService(const ctkServiceReference &reference);
    void modifiedService (const ctkServiceReference &reference, navicadVTKDicomPluginInterface* service);
    void removedService (const ctkServiceReference &reference, navicadVTKDicomPluginInterface* service);

    MainWindow* mw;
};

navicadVTKDicomPluginInterfaceTracker::navicadVTKDicomPluginInterfaceTracker(MainWindow* w) : mw(w)
{

}

navicadVTKDicomPluginInterfaceTracker::~navicadVTKDicomPluginInterfaceTracker()
{

}

navicadVTKDicomPluginInterface* navicadVTKDicomPluginInterfaceTracker::addingService(const ctkServiceReference &reference)
{
    return mw->addingService(reference);
}

void navicadVTKDicomPluginInterfaceTracker::modifiedService (const ctkServiceReference &reference, navicadVTKDicomPluginInterface* service)
{
    mw->modifiedService(reference, service);
}

void navicadVTKDicomPluginInterfaceTracker::removedService (const ctkServiceReference &reference, navicadVTKDicomPluginInterface* service)
{
    mw->modifiedService(reference, service);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_vtkDicomPluginInterfaceTracker(nullptr),
    m_ctkServiceTracker(nullptr)
{
    ui->setupUi(this);
    m_vtkTestWindow = ui->widget;

    Q_ASSERT( connect(ui->actionQuit, &QAction::triggered, qApp, &QApplication::quit) );

//    loadPlugins();
//    for(QStringList::iterator it = m_pluginNames.begin(); it != m_pluginNames.end(); ++it)
//    {
//        qDebug() << *it;
//    }

    loadPlugins();
}

MainWindow::~MainWindow()
{
    delete m_vtkDicomPluginInterfaceTracker;
    delete m_ctkServiceTracker;
    delete ui;
}

//void MainWindow::loadPlugins()
//{
//    m_pluginNames.clear();

//    QDir bdir = qApp->applicationDirPath() + "/plugins";
//    QStringList bdirFiles = bdir.entryList(QDir::Files);
//    for(QStringList::iterator it = bdirFiles.begin(); it != bdirFiles.end(); ++it)
//    {
//        QString absFilePath = bdir.absoluteFilePath(*it);
//        qDebug() << "loadPlugins -> " << absFilePath;
//        QPluginLoader loader(absFilePath);
//        QObject* plugin = loader.instance();
//        ISimpleQtPlugin* simplePlugin = qobject_cast<ISimpleQtPlugin*>(plugin);
//        if(plugin && simplePlugin)
//        {
//            m_pluginNames.push_back(*it + " - " + simplePlugin->plugin_name());
//        }
//    }
//}

void MainWindow::loadPlugins()
{
    bool succeed = ctkPluginFrameworkLauncher::start("ro.navicad.VTKDicom");
    m_vtkDicomPluginInterfaceTracker = new navicadVTKDicomPluginInterfaceTracker(this);
    ctkPluginContext* context = ctkPluginFrameworkLauncher::getPluginContext();
    m_ctkServiceTracker = new ctkServiceTracker<navicadVTKDicomPluginInterface*>(context, m_vtkDicomPluginInterfaceTracker);
    m_ctkServiceTracker->open();
//    if(succeed)
//    {
//        qDebug() << "ro.navicad.VTKDicom plugin started";

//        ctkServiceReference pluginReference = ctkPluginFrameworkLauncher::getPluginContext()->getServiceReference<navicadVTKDicomPluginInterface>();
//        navicadVTKDicomPluginInterface* pluginInterface = ctkPluginFrameworkLauncher::getPluginContext()->getService<navicadVTKDicomPluginInterface>(pluginReference);
//        if(pluginInterface != nullptr)
//        {
//            QDockWidget* dock = new QDockWidget(tr("VTK Dicom loader"), this);
//            ui->menuWindow->addAction(dock->toggleViewAction());
//            dock->setFloating(true);
//            dock->setGeometry(pluginInterface->controlWidget->geometry());
//            dock->setWidget(pluginInterface->controlWidget);
//            dock->hide();
//        }
//    }

    succeed = ctkPluginFrameworkLauncher::start("ro.navicad.CTKDicom");
    if(succeed)
    {
        qDebug() << "ro.navicad.VTKDicom plugin started";

        ctkServiceReference pluginReference = ctkPluginFrameworkLauncher::getPluginContext()->getServiceReference<navicadCTKDicomPluginInterface>();
        navicadCTKDicomPluginInterface* pluginInterface = ctkPluginFrameworkLauncher::getPluginContext()->getService<navicadCTKDicomPluginInterface>(pluginReference);
        if(pluginInterface != nullptr)
        {
            QDockWidget* dock = new QDockWidget(tr("CTK Dicom loader"), this);
            ui->menuWindow->addAction(dock->toggleViewAction());
            dock->setFloating(true);
            dock->setGeometry(pluginInterface->controlWidget->geometry());
            dock->setWidget(pluginInterface->controlWidget);
            dock->hide();
        }
    }

}

navicadVTKDicomPluginInterface* MainWindow::addingService(const ctkServiceReference &reference)
{
    ctkPluginContext* context = ctkPluginFrameworkLauncher::getPluginContext();
    navicadVTKDicomPluginInterface* pluginInterface = context->getService<navicadVTKDicomPluginInterface>(reference);

    if(pluginInterface != nullptr)
    {
        QDockWidget* dock = new QDockWidget(tr("VTK Dicom loader"), this);
        ui->menuWindow->addAction(dock->toggleViewAction());
        dock->setFloating(true);
        dock->setGeometry(pluginInterface->controlWidget->geometry());
        dock->setWidget(pluginInterface->controlWidget);
        dock->hide();
    }

    return pluginInterface;
}

void MainWindow::modifiedService (const ctkServiceReference &reference, navicadVTKDicomPluginInterface* service)
{
    if(service->dicomReader != nullptr && service->dicomData != nullptr)
    {
        qDebug() << "Dicom data added to vtk window";
        m_vtkTestWindow->addDicomData(service->dicomReader, service->dicomData);
    }
}

void MainWindow::removedService (const ctkServiceReference &reference, navicadVTKDicomPluginInterface* service)
{

}
