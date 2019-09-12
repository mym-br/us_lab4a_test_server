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

#include <iostream>
#include <string>

#include <QApplication>

#include "lzf_filter.h"
#include "ParameterMap.h"
#include "ServerWindow.h"

#define CONFIG_FILE_NAME "/config-server.txt"



int main(int argc, char *argv[])
{
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " config_dir" << std::endl;
		return EXIT_FAILURE;
	}

	if (register_lzf() < 0) {
		std::cerr << "Could not register the LZF filter (HDF5)." << std::endl;
		return EXIT_FAILURE;
	}

	const std::string configDir(argv[1]);
	const Lab::ParameterMap pm(QString(configDir.c_str()) + CONFIG_FILE_NAME);
	const std::string dataFile    = pm.value<std::string>("data_file");
	const std::string datasetName = pm.value<std::string>("dataset_name");

	QApplication a(argc, argv);
	Lab::ServerWindow w(dataFile, datasetName);
	w.show();
	return a.exec();
}
