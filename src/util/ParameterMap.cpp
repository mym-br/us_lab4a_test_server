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

#include "ParameterMap.h"



namespace Lab {

ParameterMap::ParameterMap(const QString& filePath)
		: filePath_(filePath)
		, reader_(std::make_unique<KeyValueFileReader>(filePath))
{
}

bool
ParameterMap::contains(const char* key) const
{
	const QHash<QString, QString>& map = reader_->map();
	return map.contains(key);
}

template<>
short
ParameterMap::convertValue<short>(const QString s) const
{
	bool ok;
	short v = s.toShort(&ok);
	if (!ok) {
		THROW_EXCEPTION(InvalidValueException, "Invalid short: " << s.toStdString() << '.');
	}
	return v;
}

template<>
unsigned short
ParameterMap::convertValue<unsigned short>(const QString s) const
{
	bool ok;
	unsigned short v = s.toUShort(&ok);
	if (!ok) {
		THROW_EXCEPTION(InvalidValueException, "Invalid unsigned short: " << s.toStdString() << '.');
	}
	return v;
}

template<>
int
ParameterMap::convertValue<int>(const QString s) const
{
	bool ok;
	int v = s.toInt(&ok);
	if (!ok) {
		THROW_EXCEPTION(InvalidValueException, "Invalid integer: " << s.toStdString() << '.');
	}
	return v;
}

template<>
unsigned int
ParameterMap::convertValue<unsigned int>(const QString s) const
{
	bool ok;
	unsigned int v = s.toUInt(&ok);
	if (!ok) {
		THROW_EXCEPTION(InvalidValueException, "Invalid unsigned integer: " << s.toStdString() << '.');
	}
	return v;
}

template<>
long
ParameterMap::convertValue<long>(const QString s) const
{
	bool ok;
	long v = s.toLong(&ok);
	if (!ok) {
		THROW_EXCEPTION(InvalidValueException, "Invalid long: " << s.toStdString() << '.');
	}
	return v;
}

template<>
unsigned long
ParameterMap::convertValue<unsigned long>(const QString s) const
{
	bool ok;
	unsigned long v = s.toULong(&ok);
	if (!ok) {
		THROW_EXCEPTION(InvalidValueException, "Invalid unsigned long: " << s.toStdString() << '.');
	}
	return v;
}

template<>
float
ParameterMap::convertValue<float>(const QString s) const
{
	bool ok;
	float v = s.toFloat(&ok);
	if (!ok) {
		THROW_EXCEPTION(InvalidValueException, "Invalid float: " << s.toStdString() << '.');
	}
	return v;
}

template<>
double
ParameterMap::convertValue<double>(const QString s) const
{
	bool ok;
	double v = s.toDouble(&ok);
	if (!ok) {
		THROW_EXCEPTION(InvalidValueException, "Invalid double: " << s.toStdString() << '.');
	}
	return v;
}

template<>
bool
ParameterMap::convertValue<bool>(const QString s) const
{
	if (s == "true") return true;
	if (s == "false") return false;
	THROW_EXCEPTION(InvalidValueException, "Invalid bool: " << s.toStdString() << '.');
}

template<>
std::string
ParameterMap::convertValue<std::string>(const QString s) const
{
	return s.toStdString();
}

} // namespace Lab
