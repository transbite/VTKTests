#ifndef _VTK_TESTS_H_
#define _VTK_TESTS_H_

#include <QObject>
#include <QSharedPointer>
#include <QMap>

class VolumePropertiesController;

class VtkTests : public QObject
{
    Q_OBJECT

public:
    enum class Commands
    {
        LOAD_IMAGE,
        LOAD_DICOM
    };

public:
    VtkTests(QObject* parent = nullptr);
    ~VtkTests();

    void executeCommand(const Commands& command, const QString& arguments);

signals:
    void createDockWidget(QWidget* contentWidget);
    void removeDockWidget(const QString& windowTitle);

private:
    QMap<QString, QSharedPointer<VolumePropertiesController>> m_volumeControllers;
};

#endif
