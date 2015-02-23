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


#include "GeneratedCTKPlugin_p.h"
#include <QtPlugin>

GeneratedCTKPlugin* GeneratedCTKPlugin::instance = 0;

GeneratedCTKPlugin::GeneratedCTKPlugin()
  : context(0)
{
}

GeneratedCTKPlugin::~GeneratedCTKPlugin()
{
  
}

void GeneratedCTKPlugin::start(ctkPluginContext* context)
{
  instance = this;
  this->context = context;
}

void GeneratedCTKPlugin::stop(ctkPluginContext* context)
{
  Q_UNUSED(context)
}

GeneratedCTKPlugin* GeneratedCTKPlugin::getInstance()
{
  return instance;
}

ctkPluginContext* GeneratedCTKPlugin::getPluginContext() const
{
  return context;
}

Q_EXPORT_PLUGIN2(Generated_CTK, GeneratedCTKPlugin)


