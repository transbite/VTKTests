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


#include "navicadCTKDicomPlugin.h"
#include "navicadCTKDicomPluginInterface.h"

#include <QtPlugin>
#include <QDebug>

#include <ctkFileDialog.h>
#include <ctkDICOMIndexer.h>

navicadCTKDicomPlugin* navicadCTKDicomPlugin::instance = 0;

navicadCTKDicomPlugin::navicadCTKDicomPlugin()
  : context(0)
{
}

navicadCTKDicomPlugin::~navicadCTKDicomPlugin()
{
  
}

void navicadCTKDicomPlugin::start(ctkPluginContext* context)
{
  instance = this;
  this->context = context;

  initPluginInterface();
}

void navicadCTKDicomPlugin::stop(ctkPluginContext* context)
{
  Q_UNUSED(context)
}

navicadCTKDicomPlugin* navicadCTKDicomPlugin::getInstance()
{
  return instance;
}

ctkPluginContext* navicadCTKDicomPlugin::getPluginContext() const
{
  return context;
}

void navicadCTKDicomPlugin::initPluginInterface()
{
    m_pluginInterface = new navicadCTKDicomPluginInterface;
    m_pluginInterface->controlWidget = new QWidget;
    ui.setupUi(m_pluginInterface->controlWidget);

    Q_ASSERT( connect(ui.openButton, &QPushButton::clicked, this, &navicadCTKDicomPlugin::onOpenButton) );

    ctkDictionary properties;
    properties["message"] = "CTKDicomPluginWidget";

    context->registerService<navicadCTKDicomPluginInterface>(m_pluginInterface, properties);
}

void navicadCTKDicomPlugin::onOpenButton()
{
    QString dirName = ctkFileDialog::getExistingDirectory(nullptr, tr("Open DICOM directory"), QDir::currentPath());
    qDebug() << "Dicom directory: " << dirName;

    ctkDICOMIndexer dcmIndx;
    //dcmIndx.
}

//Q_EXPORT_PLUGIN2(ro_navicad_CTKDicom, navicadCTKDicomPlugin)


