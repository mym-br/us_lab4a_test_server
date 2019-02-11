/*

  Copyright (c) 2013, 2017, 2018, 2019 Marcelo Y. Matuda.
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its
       contributors may be used to endorse or promote products derived from
       this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <string>

#include <QMainWindow>
#include <QTimer>

#include "ServerThread.h"
#include "ui_ServerWindow.h"



QT_BEGIN_NAMESPACE
class QCloseEvent;
QT_END_NAMESPACE

namespace Lab {

class ServerWindow : public QMainWindow
{
	Q_OBJECT
public:
	ServerWindow(const std::string& dataFile, const std::string& datasetName, QWidget* parent=0);
	virtual ~ServerWindow();

	void connectServer(ServerThread& server);
private:
	virtual void closeEvent(QCloseEvent* event);

	bool serverThreadEnabled_;
	QTimer logWidgetTimer_;
	ServerThread serverThread_;
	Ui::ServerWindowClass ui_;
private slots:
	void on_enableDisableButton_clicked();
	void updateLogWidget();
	void on_exitAction_triggered();
	void on_logLevelComboBox_activated(int index);
	void handleServerError();
	void handleServerFatalError();
	void handleServerInitialized();
};

} // namespace Lab

#endif // SERVERWINDOW_H
