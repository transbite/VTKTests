#pragma once

#include <QVTKWidget.h>

class VtkTestWindow : public QVTKWidget
{
public:
	VtkTestWindow(QWidget* parent = nullptr);
	~VtkTestWindow();

};

