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

#ifndef LOG_H_
#define LOG_H_

#include <sstream>

#include <QMutex>



#define LOG_ERROR ErrorLog()
#define LOG_WARNING if(Log::isWarningEnabled())WarningLog()
#define LOG_DEBUG if(Log::isDebugEnabled())DebugLog()

#define ERROR_LOG_PREFIX "ERROR >>> "
#define ERROR_LOG_SUFFIX " <<<"



namespace Lab {

/*******************************************************************************
 *
 */
class ErrorLog {
public:
	ErrorLog() { buffer_ << ERROR_LOG_PREFIX; }
	~ErrorLog();

	template<typename T> ErrorLog& operator<<(const T& item);
private:
	std::ostringstream buffer_;
};

/*******************************************************************************
 *
 */
template<typename T>
ErrorLog&
ErrorLog::operator<<(const T& item)
{
	try {
		buffer_ << item;
	} catch (...) {
		// Ignore.
	}
	return *this;
}

/*******************************************************************************
 *
 */
class WarningLog {
public:
	WarningLog() {}
	~WarningLog();

	template<typename T> WarningLog& operator<<(const T& item);
private:
	std::ostringstream buffer_;
};

/*******************************************************************************
 *
 */
template<typename T>
WarningLog&
WarningLog::operator<<(const T& item)
{
	try {
		buffer_ << item;
	} catch (...) {
		// Ignore.
	}
	return *this;
}

/*******************************************************************************
 *
 */
class DebugLog {
public:
	DebugLog() {}
	~DebugLog();

	template<typename T> DebugLog& operator<<(const T& item);
private:
	std::ostringstream buffer_;
};

/*******************************************************************************
 *
 */
template<typename T>
DebugLog&
DebugLog::operator<<(const T& item)
{
	try {
		buffer_ << item;
	} catch (...) {
		// Ignore.
	}
	return *this;
}

/*******************************************************************************
 *
 */
class Log {
public:
	enum Level {
		LEVEL_ERROR,
		LEVEL_WARNING,
		LEVEL_DEBUG
	};

	static bool isWarningEnabled() { return level_ >= LEVEL_WARNING; }
	static bool isDebugEnabled() { return level_ >= LEVEL_DEBUG; }
	static void setLevel(Level level) { level_ = level; }
	static void add(const std::ostringstream& inputBuffer);
	static void transferTo(std::string& out);
private:
	static Level level_;//TODO: use atomic
	static std::ostringstream buffer_;
	static QMutex logMutex_;

	Log() {}
	Log(const Log&);
	Log& operator=(const Log&);
};

} // namespace Lab

#endif /* LOG_H_ */
