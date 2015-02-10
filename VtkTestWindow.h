#pragma once

#include <QVTKWidget2.h>
#include <QVTKWidget.h>

class VtkTestWindow : public QVTKWidget
{
public:
    VtkTestWindow(QWidget* parent = nullptr);
	~VtkTestWindow();

};

