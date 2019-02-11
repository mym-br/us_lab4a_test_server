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

#ifndef MATRIX_H_
#define MATRIX_H_

#include <algorithm> /* swap */
#include <limits>
#include <ostream>
#include <vector>

#include "Exception.h"



namespace Lab {

template<typename T, typename Alloc=std::allocator<T>>
class Matrix {
public:
	typedef typename std::vector<T, Alloc>::iterator Iterator;
	typedef typename std::vector<T, Alloc>::const_iterator ConstIterator;
	typedef typename std::vector<T, Alloc>::size_type SizeType;
	typedef T* Pointer;
	typedef const T* ConstPointer;
	typedef T& Reference;
	typedef const T& ConstReference;
	typedef T ValueType;

	class Dim1Iterator {
	public:
		Dim1Iterator(typename Matrix<T, Alloc>::Pointer baseValuePtr, typename Matrix<T, Alloc>::SizeType increment)
			: valuePtr_(baseValuePtr)
			, increment_(increment) {
		}
		Dim1Iterator(const Dim1Iterator& iter)
			: valuePtr_(iter.valuePtr_)
			, increment_(iter.increment_) {
		}
		Dim1Iterator& operator=(const Dim1Iterator& iter) {
			if (&iter != this) {
				valuePtr_ = iter.valuePtr_;
				increment_ = iter.increment_;
			}
			return *this;
		}
		bool operator==(const Dim1Iterator& iter) const { return !(valuePtr_ != iter.valuePtr_ || increment_ != iter.increment_); }
		bool operator!=(const Dim1Iterator& iter) const { return valuePtr_ != iter.valuePtr_ || increment_ != iter.increment_; }
		Dim1Iterator& operator++() { valuePtr_ += increment_; return *this; }
		Dim1Iterator operator++(int) { Dim1Iterator iter{*this}; valuePtr_ += increment_; return iter; }
		typename Matrix<T, Alloc>::Reference operator*() { return *valuePtr_; }
	private:
		typename Matrix<T, Alloc>::Pointer valuePtr_;
		const typename Matrix<T, Alloc>::SizeType increment_;
	};

	class ConstDim1Iterator {
	public:
		ConstDim1Iterator(typename Matrix<T, Alloc>::ConstPointer baseValuePtr, typename Matrix<T, Alloc>::SizeType increment)
			: valuePtr_(baseValuePtr)
			, increment_(increment) {
		}
		ConstDim1Iterator(const ConstDim1Iterator& iter)
			: valuePtr_(iter.valuePtr_)
			, increment_(iter.increment_) {
		}
		ConstDim1Iterator& operator=(const ConstDim1Iterator& iter) {
			if (&iter != this) {
				valuePtr_ = iter.valuePtr_;
				increment_ = iter.increment_;
			}
			return *this;
		}
		bool operator==(const ConstDim1Iterator& iter) const { return !(valuePtr_ != iter.valuePtr_ || increment_ != iter.increment_); }
		bool operator!=(const ConstDim1Iterator& iter) const { return valuePtr_ != iter.valuePtr_ || increment_ != iter.increment_; }
		ConstDim1Iterator& operator++() { valuePtr_ += increment_; return *this; }
		ConstDim1Iterator operator++(int) { ConstDim1Iterator iter{*this}; valuePtr_ += increment_; return iter; }
		typename Matrix<T, Alloc>::ConstReference operator*() { return *valuePtr_; }
	private:
		typename Matrix<T, Alloc>::ConstPointer valuePtr_;
		const typename Matrix<T, Alloc>::SizeType increment_;
	};

	typedef std::pair<Dim1Iterator, Dim1Iterator> Dim1Interval;
	typedef std::pair<ConstDim1Iterator, ConstDim1Iterator> ConstDim1Interval;

	typedef Iterator Dim2Iterator;
	typedef ConstIterator ConstDim2Iterator;
	typedef std::pair<Dim2Iterator, Dim2Iterator> Dim2Interval;
	typedef std::pair<ConstDim2Iterator, ConstDim2Iterator> ConstDim2Interval;

	Matrix();
	Matrix(SizeType n1, SizeType n2);

	SizeType size() const { return data_.size(); }
	SizeType n1() const { return n1_; }
	SizeType n2() const { return n2_; }

	Reference operator()(SizeType dim1, SizeType dim2);
	ConstReference operator()(SizeType dim1, SizeType dim2) const;

	Dim1Interval dim1Interval(SizeType dim2) {
		Pointer valuePtr = &data_[dim2];
		return std::make_pair(Dim1Iterator(valuePtr, n2_), Dim1Iterator(valuePtr + n1_ * n2_, n2_));
	}

	ConstDim1Interval dim1Interval(SizeType dim2) const {
		ConstPointer valuePtr = &data_[dim2];
		return std::make_pair(ConstDim1Iterator(valuePtr, n2_), ConstDim1Iterator(valuePtr + n1_ * n2_ , n2_));
	}

	Dim2Interval dim2Interval(SizeType dim1) {
		Iterator baseIter = data_.begin() + dim1 * n2_;
		return std::make_pair(baseIter, baseIter + n2_);
	}

	ConstDim2Interval dim2Interval(SizeType dim1) const {
		ConstIterator baseIter = data_.begin() + dim1 * n2_;
		return std::make_pair(baseIter, baseIter + n2_);
	}

	void resize(SizeType n1, SizeType n2);
	void reset();
	void operator=(T value);
	bool empty() const { return data_.size() == 0; }
	void swap(Matrix& other);

	Iterator begin() { return data_.begin(); }
	Iterator end() { return data_.end(); }
	ConstIterator begin() const { return data_.begin(); }
	ConstIterator end() const { return data_.end(); }
private:
	template<typename V>
	friend std::ostream& operator<<(std::ostream& out, const Matrix<V>& m);

	void validateSize(SizeType n1, SizeType n2);

	SizeType n1_;
	SizeType n2_;
	std::vector<T, Alloc> data_;
};

template<typename T, typename Alloc>
Matrix<T, Alloc>::Matrix() : n1_(), n2_()
{
}

template<typename T, typename Alloc>
Matrix<T, Alloc>::Matrix(SizeType n1, SizeType n2) : n1_(n1), n2_(n2)
{
	validateSize(n1, n2);

	data_.resize(n1 * n2);
}

template<typename T, typename Alloc>
typename Matrix<T, Alloc>::Reference
Matrix<T, Alloc>::operator()(SizeType dim1, SizeType dim2)
{
	return data_[dim1 * n2_ + dim2];
}

template<typename T, typename Alloc>
typename Matrix<T, Alloc>::ConstReference
Matrix<T, Alloc>::operator()(SizeType dim1, SizeType dim2) const
{
	return data_[dim1 * n2_ + dim2];
}

template<typename T, typename Alloc>
void
Matrix<T, Alloc>::resize(SizeType n1, SizeType n2)
{
	if (n1 == n1_ && n2 == n2_) return;
	validateSize(n1, n2);

	n1_ = n1;
	n2_ = n2;
	data_.resize(n1 * n2);
}

template<typename T, typename Alloc>
void
Matrix<T, Alloc>::reset()
{
	n1_ = 0;
	n2_ = 0;
	data_.resize(0);
	// The memory is not deallocated.
}

template<typename T, typename Alloc>
void
Matrix<T, Alloc>::validateSize(SizeType n1, SizeType n2)
{
	if (n1 == 0) THROW_EXCEPTION(InvalidValueException, "n1 must be >= 1.");
	if (n2 == 0) THROW_EXCEPTION(InvalidValueException, "n2 must be >= 1.");

	const SizeType maxSize = std::numeric_limits<SizeType>::max() / sizeof(T);

	if (n1 < n2) {
		if (n2 > maxSize / n1) THROW_EXCEPTION(InvalidValueException, "n1*n2 must be <= " << maxSize << '.');
	} else {
		if (n1 > maxSize / n2) THROW_EXCEPTION(InvalidValueException, "n1*n2 must be <= " << maxSize << '.');
	}
}

template<typename T, typename Alloc>
void
Matrix<T, Alloc>::operator=(T value)
{
	std::fill(data_.begin(), data_.end(), value);
}

template<typename T, typename Alloc>
void
Matrix<T, Alloc>::swap(Matrix& other)
{
	std::swap(n1_, other.n1_);
	std::swap(n2_, other.n2_);
	data_.swap(other.data_);
}



template<typename T, typename Alloc>
std::ostream&
operator<<(std::ostream& out, const Matrix<T, Alloc>& m)
{
	for (typename Matrix<T, Alloc>::SizeType i = 0, n1 = m.n1(); i < n1; ++i) {
		typename Matrix<T, Alloc>::ConstDim2Interval dim2Interval = m.dim2Interval(i);
		for (typename Matrix<T, Alloc>::ConstDim2Iterator iter = dim2Interval.first; iter != dim2Interval.second; ++iter) {
			out << ' ' << *iter;
		}
		out << '\n';
	}

	return out;
}

} // namespace Lab

#endif /* MATRIX_H_ */
