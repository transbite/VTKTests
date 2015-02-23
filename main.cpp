#include "mainwindow.h"
#include <QApplication>

#include <ctkPluginFrameworkLauncher.h>
#include <ctkPluginFrameworkFactory.h>
#include <ctkPluginFramework.h>
#include <ctkPluginException.h>
#include <ctkPluginContext.h>

#include <service/event/ctkEventAdmin.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setOrganizationName("NaviTests");
    a.setOrganizationDomain("navicad.ro");
    a.setApplicationName("VTKTests");

    ctkPluginFrameworkLauncher::start("org.commontk.eventadmin");

    ctkPluginContext* pluginContext = ctkPluginFrameworkLauncher::getPluginContext();
    ctkServiceReference eventAdminRef = pluginContext->getServiceReference<ctkEventAdmin>();
    //ctkEventAdmin* eventAdmin = pluginContext->getService<ctkEventAdmin>(eventAdminRef);

    //eventAdmin->sendEvent(ctkEvent("mydomain/tutorial/CustomEvent"));

    MainWindow w;
    w.show();

    return a.exec();
}
