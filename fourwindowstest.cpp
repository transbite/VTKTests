#include "fourwindowstest.h"
#include "ui_fourwindowstest.h"

FourWindowsTest::FourWindowsTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FourWindowsTest)
{
    ui->setupUi(this);
    m_2DWindows[AXIAL] = ui->vtkAxial;
    m_2DWindows[SAGITTAL] = ui->vtkSagittal;
    m_2DWindows[CORONAL] = ui->vtkCoronal;
    m_volume = ui->volumWidget;
}

FourWindowsTest::~FourWindowsTest()
{
    delete ui;
}
