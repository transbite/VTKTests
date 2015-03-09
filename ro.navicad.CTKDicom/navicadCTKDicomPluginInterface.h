#include <QWidget>

class navicadCTKDicomPluginInterface : public QObject
{
	Q_OBJECT

public:
	QWidget* controlWidget;

};

Q_DECLARE_INTERFACE(navicadCTKDicomPluginInterface, "navicadCTKDicomPluginInterface");

