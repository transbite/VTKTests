#include <QWidget>

class navicadVTKDicomPluginInterface : public QObject
{
	Q_OBJECT

public:
	QWidget* controlWidget;

};

Q_DECLARE_INTERFACE(navicadVTKDicomPluginInterface, "navicadVTKDicomPluginInterface");
