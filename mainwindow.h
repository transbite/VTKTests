#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

namespace Ui {
class MainWindow;
}

class QDockWidget;
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
    void on_actionLoadDicom_triggered();

private:
    Ui::MainWindow *ui;
    VtkTestWindow* m_vtkTestWindow;

    QMap<QString, QDockWidget*> m_docks;
};

#endif // MAINWINDOW_H
