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


#ifndef NAVICADVTKDICOMPLUGIN_P_H
#define NAVICADVTKDICOMPLUGIN_P_H
#include "ui_pluginInterfaceWidget.h"



#include <ctkPluginActivator.h>
#include <ctkServiceRegistration.h>

class navicadVTKDicomPluginInterface;

class navicadVTKDicomPlugin :
  public QObject, public ctkPluginActivator
{
  Q_OBJECT
	Q_PLUGIN_METADATA(IID "ro.navicad.VTKDicom")
  Q_INTERFACES(ctkPluginActivator)

public:

  navicadVTKDicomPlugin();
  ~navicadVTKDicomPlugin();

  void start(ctkPluginContext* context);
  void stop(ctkPluginContext* context);

  static navicadVTKDicomPlugin* getInstance();

  ctkPluginContext* getPluginContext() const;

private slots:
  void onOpenButton();

private:
  void initPluginInterface();

private:

	Ui::pluginInterfaceWidget			ui;

	static navicadVTKDicomPlugin*		instance;
	ctkPluginContext*					context;

	navicadVTKDicomPluginInterface*		m_pluginInterface;
    ctkServiceRegistration              m_serviceRegistration;

}; // navicadVTKDicomPlugin

#endif // NAVICADVTKDICOMPLUGIN_P_H
