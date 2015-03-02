#include "mainwindow.h"
#include <QApplication>

#include <ctkPluginFrameworkLauncher.h>
#include <ctkPluginFrameworkFactory.h>
#include <ctkPluginFramework.h>
#include <ctkPluginException.h>
#include <ctkPluginContext.h>

#include <service/event/ctkEventAdmin.h>

#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setOrganizationName("navicad");
    a.setOrganizationDomain("navicad.ro");
    a.setApplicationName("VTKTests");

    ctkPluginFrameworkLauncher::addSearchPath("plugins",true);
    ctkPluginFrameworkLauncher::start();
//    bool eventadmin_started = ctkPluginFrameworkLauncher::start("org.commontk.eventadmin");
//    if(eventadmin_started)
//    {
//        qDebug() << "org.commontk.eventadmin started";
//    }

    //ctkPluginContext* pluginContext = ctkPluginFrameworkLauncher::getPluginContext();
    //ctkServiceReference eventAdminRef = pluginContext->getServiceReference<ctkEventAdmin>();
    //ctkEventAdmin* eventAdmin = pluginContext->getService<ctkEventAdmin>(eventAdminRef);

    //eventAdmin->sendEvent(ctkEvent("mydomain/tutorial/CustomEvent"));

    //test ctk generated plugin

//    bool succeed = ctkPluginFrameworkLauncher::start("Generated.CTK");
//    if(succeed)
//    {
//        qDebug() << "Generated.CTK plugin started";
//    }

//        bool succeed = ctkPluginFrameworkLauncher::start("ro.navicad.VTKDicom");
//        if(succeed)
//        {
//            qDebug() << "ro.navicad.VTKDicom plugin started";
//        }

    MainWindow w;
    w.show();

    return a.exec();
}
