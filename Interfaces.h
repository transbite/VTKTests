#ifndef _INTERFACES_H_
#define _INTERFACES_H_

#include <QtPlugin>

QT_BEGIN_NAMESPACE
class QString;
QT_END_NAMESPACE

class ISimpleQtPlugin
{
public:
    virtual ~ISimpleQtPlugin();

    virtual QString plugin_name() = 0;
};

QT_BEGIN_NAMESPACE

#define ISimpleQtPlugin_iid "tests.ISimpleQtPlugin"

Q_DECLARE_INTERFACE(ISimpleQtPlugin, ISimpleQtPlugin_iid)

QT_END_NAMESPACE

#endif //_INTERFACES_H_
