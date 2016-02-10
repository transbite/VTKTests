#include "mainwindow.h"
#include <QApplication>

#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //a.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
#ifdef __unix__
    setlocale(LC_NUMERIC,"C");
#endif

    a.setOrganizationName("navicad");
    a.setOrganizationDomain("navicad.ro");
    a.setApplicationName("VTKTests");

    MainWindow w;
    w.show();

    return a.exec();
}
