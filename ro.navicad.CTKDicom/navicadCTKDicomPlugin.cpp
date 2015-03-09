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

//Q_EXPORT_PLUGIN2(ro_navicad_CTKDicom, navicadCTKDicomPlugin)


