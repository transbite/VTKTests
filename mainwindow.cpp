#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Interfaces.h"

#include "ro.navicad.VTKDicom/navicadVTKDicomPluginInterface.h"
#include "ro.navicad.CTKDicom/navicadCTKDicomPluginInterface.h"

#include <QDir>
#include <QApplication>
#include <QPluginLoader>

#include <ctkPluginFrameworkLauncher.h>
#include <ctkPluginFrameworkFactory.h>
#include <ctkPluginFramework.h>
#include <ctkPluginException.h>
#include <ctkPluginContext.h>

#include <QDockWidget>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
    if(succeed)
    {
        qDebug() << "ro.navicad.VTKDicom plugin started";

        ctkServiceReference pluginReference = ctkPluginFrameworkLauncher::getPluginContext()->getServiceReference<navicadVTKDicomPluginInterface>();
        navicadVTKDicomPluginInterface* pluginInterface = ctkPluginFrameworkLauncher::getPluginContext()->getService<navicadVTKDicomPluginInterface>(pluginReference);
        if(pluginInterface != nullptr)
        {
            QDockWidget* dock = new QDockWidget(tr("VTK Dicom loader"), this);
            ui->menuWindow->addAction(dock->toggleViewAction());
            dock->setFloating(true);
            dock->setGeometry(pluginInterface->controlWidget->geometry());
            dock->setWidget(pluginInterface->controlWidget);
            dock->hide();
        }
    }

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
