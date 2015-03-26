/*=============================================================================

  Library: CTK

  Copyright (c) navicad

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

=============================================================================*/


#include "navicadVTKDicomPlugin.h"
#include "navicadVTKDicomPluginInterface.h"

#include <QtPlugin>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

#include <vtkDICOMImageReader.h>
#include <vtkImageData.h>
#include <vtkInformation.h>

QString navicadVTKDicomPluginInterface::LABEL_WIDGET    = "VTKDicomPluginInterfaceWidget";
QString navicadVTKDicomPluginInterface::LABEL_READER    = "VTKDicomPluginInterfaceReader";
QString navicadVTKDicomPluginInterface::LABEL_DATA      = "VTKDicomPluginInterfaceData";

navicadVTKDicomPlugin* navicadVTKDicomPlugin::instance = 0;

navicadVTKDicomPlugin::navicadVTKDicomPlugin()
  : context(0)
{
    qDebug() << "navicadVTKDicomPlugin constructor";
}

navicadVTKDicomPlugin::~navicadVTKDicomPlugin()
{
  
}

void navicadVTKDicomPlugin::start(ctkPluginContext* context)
{
  instance = this;
  this->context = context;
  qDebug() << "navicadVTKDicomPlugin start";

  initPluginInterface();
}

void navicadVTKDicomPlugin::stop(ctkPluginContext* context)
{
  Q_UNUSED(context)
    qDebug() << "navicadVTKDicomPlugin stop";
}

navicadVTKDicomPlugin* navicadVTKDicomPlugin::getInstance()
{
  return instance;
}

ctkPluginContext* navicadVTKDicomPlugin::getPluginContext() const
{
  return context;
}

void navicadVTKDicomPlugin::initPluginInterface()
{
    m_pluginInterface = new navicadVTKDicomPluginInterface;
    m_pluginInterface->controlWidget = new QWidget;
    m_pluginInterface->dicomReader = nullptr;
    m_pluginInterface->dicomData = nullptr;
    ui.setupUi(m_pluginInterface->controlWidget);

    Q_ASSERT( connect(ui.openButton, &QPushButton::clicked, this, &navicadVTKDicomPlugin::onOpenButton) );

    ctkDictionary properties;
    properties["message"] = "VTKDicomPluginWidget";

    m_serviceRegistration = context->registerService<navicadVTKDicomPluginInterface>(m_pluginInterface, properties);
}

void navicadVTKDicomPlugin::onOpenButton()
{
    QString dirName = QFileDialog::getExistingDirectory(nullptr, tr("Open DICOM directory"), QDir::currentPath());
    if(dirName.isEmpty())
    {
        return;
    }

    vtkDICOMImageReader* dicomReader = vtkDICOMImageReader::New();
    dicomReader->SetDirectoryName(dirName.toLatin1().constData());
    dicomReader->Update();

    vtkImageData* dicomData = dicomReader->GetOutput();
    int dim[3] = {0, 0, 0};
    if(dicomData != nullptr)
    {
        dicomData->GetDimensions(dim);
    }
    if(dim[0] < 2 || dim[1] < 2 || dim[2] < 2)
    {
        QMessageBox::warning(nullptr, tr("Dicom loader"), tr("Error loading dicom data!"), QMessageBox::Ok);
        return;
    }
    QMessageBox::information(nullptr, tr("Dicom loader"), tr("Dicom data loaded."), QMessageBox::Ok);

    QString patientName = dicomReader->GetPatientName();
    QString studyId = dicomReader->GetStudyID();
    qDebug() << "Patient name: " << patientName << "; Study id: " << studyId;
    vtkInformation* dicomInformation = dicomReader->GetInformation();
    vtkIndent indent(2);
    std::cout << "Dicom information: " << endl;
    dicomInformation->PrintHeader(std::cout, indent);
    dicomInformation->PrintKeys(std::cout, indent);

    //TODO - clean old data
    //m_pluginInterface->dicomReader->Delete();
    //m_pluginInterface->dicomData->Delete();
    m_pluginInterface->dicomReader = dicomReader;
    m_pluginInterface->dicomData = dicomData;
    ctkDictionary properties;
    properties["message"] = "VTKDicomPluginData";
    m_serviceRegistration.setProperties(properties);
}

//Q_EXPORT_PLUGIN2(ro_navicad_VTKDicom, navicadVTKDicomPlugin)


