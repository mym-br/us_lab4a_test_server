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

#ifndef PARAMETERMAP_H
#define PARAMETERMAP_H

#include <memory>

#include <QHash>
#include <QString>

#include "Exception.h"
#include "KeyValueFileReader.h"



namespace Lab {

class ParameterMap {
public:
	ParameterMap(const QString& filePath);

	bool contains(const char* key) const;
	template<typename T> T value(const char* key) const;
	template<typename T> T value(const char* key, T minValue, T maxValue) const;
private:
	ParameterMap(const ParameterMap&) = delete;
	ParameterMap& operator=(const ParameterMap&) = delete;

	template<typename T> T convertValue(const QString s) const;

	const QString filePath_;
	std::unique_ptr<const KeyValueFileReader> reader_;
};

typedef std::shared_ptr<const ParameterMap> ConstParameterMapPtr;



template<typename T>
T
ParameterMap::value(const char* key) const
{
	const QHash<QString, QString>& map = reader_->map();

	if (!map.contains(key)) {
		THROW_EXCEPTION(InvalidValueException, "Key '" << key << "' not found in file " << filePath_.toStdString() << '.');
	}

	QString valueString = map.value(key);
	T value;
	try {
		value = convertValue<T>(valueString);
	} catch (const std::exception& e) {
		THROW_EXCEPTION(InvalidValueException, "Invalid value for key '" << key << "' in file " << filePath_.toStdString() << ": " << e.what());
	}
	return value;
}

template<typename T>
T
ParameterMap::value(const char* key, T minValue, T maxValue) const
{
	T v = value<T>(key);

	if (v > maxValue) {
		THROW_EXCEPTION(InvalidValueException, "The value for key '" << key << "' must be <= " << maxValue << " in file " << filePath_.toStdString() << '.');
	} else if (v < minValue) {
		THROW_EXCEPTION(InvalidValueException, "The value for key '" << key << "' must be >= " << minValue << " in file " << filePath_.toStdString() << '.');
	}

	return v;
}

} // namespace Lab

#endif // PARAMETERMAP_H
