#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

namespace Ui {
class MainWindow;
}

class QDockWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void loadPlugins();

private:
    Ui::MainWindow *ui;
//    QStringList m_pluginNames;
    QMap<QString, QDockWidget*> m_docks;
};

#endif // MAINWINDOW_H
