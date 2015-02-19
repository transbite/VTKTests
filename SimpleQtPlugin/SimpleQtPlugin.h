#ifndef _SIMPLE_QT_PLUGIN_H_
#define _SIMPLE_QT_PLUGIN_H_

#include "Interfaces.h"

#include <QString>

class SimpleQtPlugin
        : public QObject
        , public ISimpleQtPlugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "tests.ISimpleQtPlugin" FILE "SimpleQtPlugin.json")

    Q_INTERFACES(ISimpleQtPlugin)

public:
    QString plugin_name() Q_DECL_OVERRIDE;

private:

};

#endif //_SIMPLE_QT_PLUGIN_H_
