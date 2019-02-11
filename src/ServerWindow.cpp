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

#include "ServerWindow.h"

#include <QScrollBar>
#include <QString>

#include "Log.h"
#include "LogSyntaxHighlighter.h"

#define LOG_TIMER_PERIOD_MS 200
#define MAX_LOG_BLOCK_COUNT 500
#define DEFAULT_PORT_NUMBER 55500
#define MINIMUM_PORT_NUMBER 49152
#define MAXIMUM_PORT_NUMBER 65535



namespace Lab {

ServerWindow::ServerWindow(const std::string& dataFile, const std::string& datasetName, QWidget* parent)
		: QMainWindow(parent)
		, serverThreadEnabled_(false)
		, logWidgetTimer_(this)
		, serverThread_(dataFile, datasetName, this)
{
	ui_.setupUi(this);

	ui_.logPlainTextEdit->setMaximumBlockCount(MAX_LOG_BLOCK_COUNT);

	logWidgetTimer_.start(LOG_TIMER_PERIOD_MS);
	connect(&logWidgetTimer_, SIGNAL(timeout()), this, SLOT(updateLogWidget()));

	new LogSyntaxHighlighter(ui_.logPlainTextEdit->document());

	ui_.portNumberSpinBox->setMinimum(MINIMUM_PORT_NUMBER);
	ui_.portNumberSpinBox->setMaximum(MAXIMUM_PORT_NUMBER);
	ui_.portNumberSpinBox->setValue(DEFAULT_PORT_NUMBER);

	ui_.logLevelComboBox->addItem(tr("Error"), Log::LEVEL_ERROR);
	ui_.logLevelComboBox->addItem(tr("Warning"), Log::LEVEL_WARNING);
	ui_.logLevelComboBox->addItem(tr("Debug"), Log::LEVEL_DEBUG);
	ui_.logLevelComboBox->setCurrentIndex(2);

	serverThread_.start();
}

ServerWindow::~ServerWindow()
{
}

void
ServerWindow::updateLogWidget()
{
	std::string s;
	Log::transferTo(s);
	if (s.length() > 0) {
		ui_.logPlainTextEdit->appendPlainText(QString::fromStdString(s));

		//ui_.logPlainTextEdit->ensureCursorVisible();
		QScrollBar* scrollBar = ui_.logPlainTextEdit->verticalScrollBar();
		scrollBar->setValue(scrollBar->maximum());
	}
}

void
ServerWindow::on_enableDisableButton_clicked()
{
//	LOG_DEBUG << "on_enableDisableButton_clicked";

	if (!serverThread_.isRunning()) {
		return;
	}

	if (serverThreadEnabled_) {
		LOG_DEBUG << "DISABLE";
		serverThread_.disableServer();
		serverThreadEnabled_ = false;
		ui_.enableDisableButton->setText(tr("Enable"));
	} else {
		LOG_DEBUG << "ENABLE";
		serverThread_.enableServer(ui_.portNumberSpinBox->value());
		serverThreadEnabled_ = true;
		ui_.enableDisableButton->setText(tr("Disable"));
	}
}

void
ServerWindow::on_exitAction_triggered()
{
	qApp->closeAllWindows();
}

void
ServerWindow::on_logLevelComboBox_activated(int index)
{
	Log::setLevel(static_cast<Log::Level>(ui_.logLevelComboBox->itemData(index).toInt()));
}

void
ServerWindow::closeEvent(QCloseEvent* event)
{
	if (serverThread_.isRunning()) {
		serverThread_.exitLoop();
	}

	event->ignore();
	qApp->quit();
}

void
ServerWindow::handleServerError()
{
	serverThreadEnabled_ = false;
	ui_.enableDisableButton->setText(tr("Enable"));
}

void
ServerWindow::handleServerFatalError()
{
	serverThreadEnabled_ = false;
	ui_.enableDisableButton->setText(tr("Enable"));
	ui_.enableDisableButton->setEnabled(false);
}

void
ServerWindow::handleServerInitialized()
{
	ui_.enableDisableButton->setEnabled(true);
}

void
ServerWindow::connectServer(ServerThread& server)
{
	connect(&server, SIGNAL(errorOcurred()), this, SLOT(handleServerError()));
	connect(&server, SIGNAL(fatalErrorOcurred()), this, SLOT(handleServerFatalError()));
	connect(&server, SIGNAL(initialized()), this, SLOT(handleServerInitialized()));
}

} // namespace Lab
