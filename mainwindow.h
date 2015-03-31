#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <ctkServiceReference.h>
#include <ctkServiceTracker.h>

namespace Ui {
class MainWindow;
}

class QDockWidget;
class navicadVTKDicomPluginInterface;
class navicadVTKDicomPluginInterfaceTracker;
class VtkTestWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    friend class navicadVTKDicomPluginInterfaceTracker;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionLoadImage_triggered();

private:
    void loadPlugins();

private:
    navicadVTKDicomPluginInterface* addingService(const ctkServiceReference &reference);
    void modifiedService (const ctkServiceReference &reference, navicadVTKDicomPluginInterface* service);
    void removedService (const ctkServiceReference &reference, navicadVTKDicomPluginInterface* service);

private:
    Ui::MainWindow *ui;
    VtkTestWindow* m_vtkTestWindow;
    navicadVTKDicomPluginInterfaceTracker* m_vtkDicomPluginInterfaceTracker;
    ctkServiceTracker<navicadVTKDicomPluginInterface*>* m_ctkServiceTracker;
//    QStringList m_pluginNames;
    QMap<QString, QDockWidget*> m_docks;
};

#endif // MAINWINDOW_H
