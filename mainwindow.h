#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

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
};

#endif // MAINWINDOW_H
