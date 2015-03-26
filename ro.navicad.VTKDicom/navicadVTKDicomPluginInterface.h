#include <QWidget>
#include <vtkDICOMImageReader.h>
#include <vtkImageData.h>

class navicadVTKDicomPluginInterface : public QObject
{
	Q_OBJECT

public:

	static QString LABEL_WIDGET;
	static QString LABEL_READER;
	static QString LABEL_DATA;

	QWidget*				controlWidget;
	vtkDICOMImageReader*	dicomReader;
	vtkImageData*			dicomData;
};

Q_DECLARE_INTERFACE(navicadVTKDicomPluginInterface, "navicadVTKDicomPluginInterface");
