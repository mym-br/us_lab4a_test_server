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

#ifndef HDF5UTIL_H_
#define HDF5UTIL_H_

#include <cstddef> /* std::size_t */
#include <string>

#include <H5Cpp.h>

#include "Exception.h"
#include "Matrix.h"



namespace Lab {
namespace HDF5Util {

#ifndef H5_NO_NAMESPACE
using namespace H5;
#endif

template<typename TFloat> void resize(Matrix<TFloat>& container, hsize_t n1, hsize_t n2);
template<typename TFloat> TFloat* getBeginPtr(Matrix<TFloat>& container);
template<typename T> void load2(const std::string& filePath, const std::string& dataSetName, T& container);
template<typename T> PredType hdf5MemoryType();



template<typename TFloat>
void
resize(Matrix<TFloat>& container, hsize_t n1, hsize_t n2)
{
	if (n1 == 0) {
		THROW_EXCEPTION(InvalidParameterException, "The first dimension (" << n1 << ") is equal to 0.");
	}
	if (n2 == 0) {
		THROW_EXCEPTION(InvalidParameterException, "The second dimension (" << n2 << ") is equal to 0.");
	}
	container.resize(n1, n2);
}

template<typename TFloat>
TFloat*
getBeginPtr(Matrix<TFloat>& m)
{
	return &m(0, 0);
}

template<typename T>
void
load2(const std::string& filePath, const std::string& dataSetName, T& container)
{
	try {
		H5::Exception::dontPrint();

		H5File file(filePath, H5F_ACC_RDONLY);

		DataSet dataSet = file.openDataSet(dataSetName);
		H5T_class_t typeClass = dataSet.getTypeClass();
		if (typeClass != H5T_FLOAT) {
			THROW_EXCEPTION(InvalidFileException, "The data type class in the file " << filePath << " is not floating point.");
		}

		H5::FloatType type = dataSet.getFloatType();
		//H5std_string orderString;
		//H5T_order_t order = type.getOrder(orderString);
		//std::cout << orderString << std::endl;
		std::size_t typeSize = type.getSize();
		if (typeSize != sizeof(double)) {
			THROW_EXCEPTION(InvalidFileException, "The data type in the file " << filePath << " is not double.");
		}

		DataSpace dataSetSpace = dataSet.getSpace();
		if (!dataSetSpace.isSimple()) {
			THROW_EXCEPTION(InvalidFileException, "The dataset in the file " << filePath << " is not simple.");
		}

		int rank = dataSetSpace.getSimpleExtentNdims();
		switch (rank) {
		case 1:
			{
				hsize_t dim;
				dataSetSpace.getSimpleExtentDims(&dim);
				resize(container, 1, dim);
			}
			break;
		case 2:
			{
				hsize_t dims[2];
				dataSetSpace.getSimpleExtentDims(dims);
				resize(container, dims[0], dims[1]);
			}
			break;
		default:
			THROW_EXCEPTION(InvalidFileException, "The rank in the file " << filePath << " is not 1 or 2.");
		}

		dataSet.read(getBeginPtr(container), hdf5MemoryType<T>());

	} catch (const H5::Exception& e) {
		THROW_EXCEPTION(IOException, "An error ocurred in HDF5 library with file " << filePath << ": " << e.getCDetailMsg());
	}
}

} // namespace HDF5Util
} // namespace Lab

#endif /* HDF5UTIL_H_ */
