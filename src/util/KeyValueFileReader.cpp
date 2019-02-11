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

#include "KeyValueFileReader.h"

#include <QFile>
#include <QStringList>
#include <QTextStream>

#include "Exception.h"



namespace Lab {

KeyValueFileReader::KeyValueFileReader(const QString& filePath)
{
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		THROW_EXCEPTION(IOException, "The file " << filePath.toStdString() << " could not be opened.");
	}

	QTextStream in(&file);
	in.setCodec("UTF-8");
	int lineNumber = 0;
	while (!in.atEnd()) {
		++lineNumber;

		QString line = in.readLine().trimmed();

		if (line.startsWith('#')) continue; // comment
		if (line.isEmpty()) continue;

		QStringList itemList = line.split('='/*, QString::SkipEmptyParts*/);
		if (itemList.size() == 1) {
			THROW_EXCEPTION(InvalidValueException, "Missing '=' separator at line " << lineNumber << " of file " << filePath.toStdString() << '.');
		}
		if (itemList.size() != 2) {
			THROW_EXCEPTION(InvalidValueException, "Invalid syntax at line " << lineNumber << " of file " << filePath.toStdString() << '.');
		}

		QString key = itemList.at(0).trimmed();
		if (key.isEmpty()) {
			THROW_EXCEPTION(InvalidValueException, "Empty key at line " << lineNumber << " of file " << filePath.toStdString() << '.');
		}
		QString value = itemList.at(1).trimmed();
		if (value.isEmpty()) {
			THROW_EXCEPTION(InvalidValueException, "Empty value at line " << lineNumber << " of file " << filePath.toStdString() << '.');
		}
		if (map_.contains(key)) {
			THROW_EXCEPTION(InvalidValueException, "Duplicated key '" << key.toStdString() << "' at line " << lineNumber << " of file " << filePath.toStdString() << '.');
		}
		map_.insert(key, value);
	}
}

KeyValueFileReader::~KeyValueFileReader()
{
}

} // namespace Lab
