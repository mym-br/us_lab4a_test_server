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

#ifndef UTIL_H_
#define UTIL_H_

#include <cmath>
#include <vector>
#include <ctime> /* nanosleep */

#include "Matrix.h"



namespace Lab {
namespace Util {

void sleepMs(unsigned long milliseconds);

template<typename T> T multiplyElements(const std::vector<T>& v);
template<typename T> void normalize(T& data);
template<typename T> void multiply(Matrix<T>& data, T coefficient);
template<typename T> T maxAbsolute(const Matrix<T>& data);



inline
void
sleepMs(unsigned long milliseconds)
{
	struct timespec tspec;
	if (milliseconds >= 1000) {
		tspec.tv_sec = milliseconds / 1000;
		tspec.tv_nsec = (milliseconds % 1000) * 1000000;
	} else {
		tspec.tv_sec = 0;
		tspec.tv_nsec = milliseconds * 1000000;
	}
	nanosleep(&tspec, 0);
}

template<typename T>
T
multiplyElements(const std::vector<T>& v)
{
	T res = 1;
	for (typename std::vector<T>::const_iterator iter = v.begin(); iter != v.end(); ++iter) {
		res *= *iter;
	}
	return res;
}

template<typename T>
void
normalize(T& data)
{
	const auto maxAbs = maxAbsolute(data);
	if (maxAbs == 0) return;
	const auto coeff = 1 / maxAbs;
	multiply(data, coeff);
}

template<typename T>
void
multiply(Matrix<T>& data, T coefficient)
{
	for (typename Matrix<T>::Iterator iter = data.begin(); iter != data.end(); ++iter) {
		*iter *= coefficient;
	}
}

template<typename T>
T
maxAbsolute(const Matrix<T>& data)
{
	T max = 0;
	for (typename Matrix<T>::ConstIterator iter = data.begin(); iter != data.end(); ++iter) {
		const T a = std::abs(*iter);
		if (max < a) max = a;
	}
	return max;
}

} // namespace Util
} // namespace Lab

#endif /* UTIL_H_ */
