#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Interfaces.h"

#include <QDir>
#include <QApplication>
#include <QPluginLoader>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loadPlugins();
    for(QStringList::iterator it = m_pluginNames.begin(); it != m_pluginNames.end(); ++it)
    {
        qDebug() << *it;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadPlugins()
{
    m_pluginNames.clear();

    QDir bdir = qApp->applicationDirPath() + "/plugins";
    QStringList bdirFiles = bdir.entryList(QDir::Files);
    for(QStringList::iterator it = bdirFiles.begin(); it != bdirFiles.end(); ++it)
    {
        QString absFilePath = bdir.absoluteFilePath(*it);
        qDebug() << "loadPlugins -> " << absFilePath;
        QPluginLoader loader(absFilePath);
        QObject* plugin = loader.instance();
        ISimpleQtPlugin* simplePlugin = qobject_cast<ISimpleQtPlugin*>(plugin);
        if(plugin && simplePlugin)
        {
            m_pluginNames.push_back(*it + " - " + simplePlugin->plugin_name());
        }
    }
}
