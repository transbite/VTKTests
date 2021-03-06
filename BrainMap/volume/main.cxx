// Copyright 2013 Allen Institute for Brain Science
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <QApplication>
#include "MainWindow.h"

// This is the standard main function of a Qt application.  It creates a 
// generic application and the custom UI (MainWindow), then starts the app.
int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	
	MainWindow win;
	win.show();
	
	return app.exec();
}
