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
#include <QDebug>

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
    ui.setupUi(m_pluginInterface->controlWidget);

    Q_ASSERT( connect(ui.openButton, &QPushButton::clicked, this, &navicadVTKDicomPlugin::onOpenButton) );

    ctkDictionary properties;
    properties["message"] = "VTKDicomPluginWidget";

    context->registerService<navicadVTKDicomPluginInterface>(m_pluginInterface, properties);
}

void navicadVTKDicomPlugin::onOpenButton()
{

}

//Q_EXPORT_PLUGIN2(ro_navicad_VTKDicom, navicadVTKDicomPlugin)


