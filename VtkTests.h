#ifndef _VTK_TESTS_H_
#define _VTK_TESTS_H_

#include <QObject>

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
    VtkTests();
    ~VtkTests();

    void executeCommand(const Commands& command, const QString& arguments);

};

#endif
