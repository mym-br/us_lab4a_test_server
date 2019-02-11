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

#include "ServerThread.h"

#include <QMutexLocker>

#include "ArrayAcqServer.h"
#include "Log.h"
#include "ServerWindow.h"
#include "TestDevice.h"

#define SLEEP_TIME_MS 1000



namespace Lab {

ServerThread::ServerThread(const std::string& dataFile, const std::string& datasetName, ServerWindow* serverWindow)
		: QThread(serverWindow)
		, dataFile_(dataFile)
		, datasetName_(datasetName)
		, state_(STATE_DISABLED)
		, portNumber_()
		, acqDevice_()
		, serverWindow_(serverWindow)
{
}

ServerThread::~ServerThread()
{
}

void
ServerThread::run()
{
	LOG_DEBUG << "Server thread started.";

	if (serverWindow_) {
		serverWindow_->connectServer(*this);
	}

	try {
		acqDevice_.reset(new TestDevice(dataFile_, datasetName_));
	} catch (std::exception& e) {
		LOG_ERROR << "Error [" << typeid(e).name() << "]: " << e.what();
		emit fatalErrorOcurred();
		return;
	} catch (...) {
		LOG_ERROR << "Unknown error.";
		emit fatalErrorOcurred();
		return;
	}

	emit initialized();

	LOG_DEBUG << "Test device open.";

	for (;;) {
		State localState;
		unsigned short localPortNumber;
		{
			QMutexLocker locker(&mutex_);
			if (state_ == STATE_EXITING) {
				break;
			}

			while (state_ == STATE_DISABLED) {
				LOG_DEBUG << "Server thread: WAIT";
				condition_.wait(&mutex_);
			}
			localState = state_;
			localPortNumber = portNumber_;
		}

		if (localState == STATE_ENABLED) {
			LOG_DEBUG << "Server thread: EXECUTING";
			try {
				if (server_) server_.reset(); // closes an open connection
				server_.reset(new ArrayAcqServer<TestDevice>(localPortNumber, *acqDevice_));
				server_->exec();
			} catch (std::bad_alloc& /*e*/) {
				disableServer();
				emit errorOcurred();
				LOG_ERROR << "Error: Out of memory.";
			} catch (std::exception& e) {
				disableServer();
				emit errorOcurred();
				LOG_ERROR << "Error [" << typeid(e).name() << "]: " << e.what();
			} catch (...) {
				disableServer();
				emit errorOcurred();
				LOG_ERROR << "Unknown error.";
			}
		}

		msleep(SLEEP_TIME_MS);
	}

	acqDevice_.reset();
}

void
ServerThread::enableServer(unsigned short portNumber)
{
	QMutexLocker locker(&mutex_);
	if (state_ == STATE_DISABLED) {
		state_ = STATE_ENABLED;
		portNumber_ = portNumber;
		condition_.wakeOne();
	}
}

void
ServerThread::disableServer()
{
	QMutexLocker locker(&mutex_);
	if (state_ == STATE_ENABLED) {
		state_ = STATE_DISABLED;
		portNumber_ = 0;

		if (server_) server_->stop();
	}
}

void
ServerThread::exitLoop()
{
	{
		QMutexLocker locker(&mutex_);
		state_ = STATE_EXITING;
		condition_.wakeOne();
	}

	if (server_) server_->stop();

	wait();
}

} // namespace Lab
