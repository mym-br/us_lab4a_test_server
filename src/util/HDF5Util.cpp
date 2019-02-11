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

#include "HDF5Util.h"

#include <algorithm> /* max_element, min */

#include "Util.h"

#define HDF5UTIL_MAX_CHUNK_SIZE_2D 64
#define HDF5UTIL_MAX_CHUNK_SIZE_1D (64 * 64 * 2)



namespace Lab {
namespace HDF5Util {

template<>
PredType
hdf5MemoryType<std::vector<float>>()
{
	return PredType::NATIVE_FLOAT;
}

template<>
PredType
hdf5MemoryType<std::vector<std::pair<float, float>>>()
{
	return PredType::NATIVE_FLOAT;
}

template<>
PredType
hdf5MemoryType<Matrix<float>>()
{
	return PredType::NATIVE_FLOAT;
}

template<>
PredType
hdf5MemoryType<std::vector<double>>()
{
	return PredType::NATIVE_DOUBLE;
}

template<>
PredType
hdf5MemoryType<std::vector<std::pair<double, double>>>()
{
	return PredType::NATIVE_DOUBLE;
}

template<>
PredType
hdf5MemoryType<Matrix<double>>()
{
	return PredType::NATIVE_DOUBLE;
}

template<>
PredType
hdf5MemoryType<std::vector<unsigned int>>()
{
	return PredType::NATIVE_UINT;
}

void
calcChunkDims(const std::vector<hsize_t>& dataDims, std::vector<hsize_t>& chunkDims)
{
	chunkDims.assign(dataDims.size(), 0);
	unsigned int rank = dataDims.size();
	switch (rank) {
	case 1:
		chunkDims[0] = std::min<hsize_t>(dataDims[0], HDF5UTIL_MAX_CHUNK_SIZE_1D);
		return;
	case 2:
		if (dataDims[0] == 1) {
			chunkDims[0] = 1;
			chunkDims[1] = std::min<hsize_t>(dataDims[1], HDF5UTIL_MAX_CHUNK_SIZE_1D);
			return;
		} else if (dataDims[1] == 1) {
			chunkDims[0] = std::min<hsize_t>(dataDims[0], HDF5UTIL_MAX_CHUNK_SIZE_1D);
			chunkDims[1] = 1;
			return;
		} else if (dataDims[0] >= HDF5UTIL_MAX_CHUNK_SIZE_2D && dataDims[1] >= HDF5UTIL_MAX_CHUNK_SIZE_2D) {
			chunkDims[0] = std::min<hsize_t>(dataDims[0], HDF5UTIL_MAX_CHUNK_SIZE_2D);
			chunkDims[1] = std::min<hsize_t>(dataDims[1], HDF5UTIL_MAX_CHUNK_SIZE_2D);
			return;
		} else {
			chunkDims = dataDims;
			hsize_t size = Util::multiplyElements(chunkDims);
			while (size > HDF5UTIL_MAX_CHUNK_SIZE_1D) {
				std::vector<hsize_t>::iterator maxElement = std::max_element(chunkDims.begin(), chunkDims.end());
				*maxElement /= 2;
				size = Util::multiplyElements(chunkDims);
			}
			return;
		}
	default:
		THROW_EXCEPTION(InvalidValueException, "Invalid rank: " << rank << '.');
	}
}

} // namespace HDF5Util
} // namespace Lab
