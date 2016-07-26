#include "mainwindow.h"
#include <QApplication>

#ifdef Q_OS_OSX
#include <QSurfaceFormat>
#endif

#include <QDebug>

int main(int argc, char *argv[])
{
#ifdef Q_OS_OSX
    QSurfaceFormat surfaceFormat;
    surfaceFormat.setVersion(4, 1);
    surfaceFormat.setProfile(QSurfaceFormat::CoreProfile);
    //surfaceFormat.setSamples(4);
    surfaceFormat.setRenderableType(QSurfaceFormat::OpenGL);
    //surfaceFormat.setSwapBehavior(QSurfaceFormat::TripleBuffer);
    surfaceFormat.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    QSurfaceFormat::setDefaultFormat(surfaceFormat);
#endif


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
