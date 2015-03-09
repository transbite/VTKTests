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


#ifndef NAVICADCTKDICOMPLUGIN_H
#define NAVICADCTKDICOMPLUGIN_H

#include "ui_pluginInterfaceWidget.h"

#include <ctkPluginActivator.h>

class navicadCTKDicomPluginInterface;

class navicadCTKDicomPlugin :
  public QObject, public ctkPluginActivator
{
  Q_OBJECT
    Q_PLUGIN_METADATA(IID "ro.navicad.CTKDicom")
  Q_INTERFACES(ctkPluginActivator)

public:

  navicadCTKDicomPlugin();
  ~navicadCTKDicomPlugin();

  void start(ctkPluginContext* context);
  void stop(ctkPluginContext* context);

  static navicadCTKDicomPlugin* getInstance();

  ctkPluginContext* getPluginContext() const;


private:
    Ui::pluginInterfaceWidget			ui;

  static navicadCTKDicomPlugin* instance;
  ctkPluginContext* context;

  navicadCTKDicomPluginInterface*		m_pluginInterface;

}; // navicadCTKDicomPlugin

#endif // NAVICADCTKDICOMPLUGIN_H
