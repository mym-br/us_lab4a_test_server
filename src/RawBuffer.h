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

#ifndef RAWBUFFER_H_
#define RAWBUFFER_H_

#include <cstddef> /* std::size_t */
#include <cstring>
#include <string>
#include <vector>

#include <boost/cstdint.hpp>

#include "Exception.h"



namespace Lab {

/*******************************************************************************
 *
 */
class RawBuffer {
public:
	typedef boost::uint8_t* iterator;
	typedef const boost::uint8_t* const_iterator;

	enum {
		INITIAL_RESERVED_SIZE = 8192
	};

	RawBuffer() : readIndex_() {
		buffer_.reserve(INITIAL_RESERVED_SIZE);
	}
	~RawBuffer() {}

	void reset();
	void reserve(std::size_t size);

	void putInt16(boost::int16_t value);
	boost::int16_t getInt16();

	void putUInt32(boost::uint32_t value);
	boost::uint32_t getUInt32();

	void putFloat(float value);
	float getFloat();

	void putString(const std::string& s);
	void getString(std::string& s);

	void putFloatArray(const std::vector<float>& a);
	void getFloatArray(std::vector<float>& a);

	template<typename T> void putInt16Array(const std::vector<T>& a);
	template<typename T> void putInt16Array(const T* a, std::size_t arraySize);
	template<typename T> void getInt16Array(std::vector<T>& a);
	template<typename T> void getInt16Array(T* a, std::size_t arraySize);

	boost::uint8_t& front()
	{
		return buffer_.front();
	}

	const boost::uint8_t& front() const
	{
		return buffer_.front();
	}

	std::size_t size() const
	{
		return buffer_.size();
	}
private:
	RawBuffer(const RawBuffer&);
	RawBuffer& operator=(const RawBuffer&);

	static void writeUInt32(boost::uint32_t value, std::vector<boost::uint8_t>&buffer, std::size_t& index);
	static boost::uint32_t readUInt32(const std::vector<boost::uint8_t>&buffer, std::size_t& index);
	static void writeInt16(boost::int16_t value, std::vector<boost::uint8_t>&buffer, std::size_t& index);
	static boost::int16_t readInt16(const std::vector<boost::uint8_t>&buffer, std::size_t& index);

	std::size_t readIndex_;
	std::vector<boost::uint8_t> buffer_; // big-endian
};

/*******************************************************************************
 *
 */
inline
void
RawBuffer::writeUInt32(boost::uint32_t value, std::vector<boost::uint8_t>&buffer, std::size_t& index)
{
	buffer[index++] = value >> 24;
	buffer[index++] = value >> 16;
	buffer[index++] = value >> 8;
	buffer[index++] = value;
}

/*******************************************************************************
 *
 */
inline
boost::uint32_t
RawBuffer::readUInt32(const std::vector<boost::uint8_t>&buffer, std::size_t& index)
{
	boost::uint32_t value = buffer[index++] << 24;
	value                += buffer[index++] << 16;
	value                += buffer[index++] << 8;
	value                += buffer[index++];
	return value;
}

/*******************************************************************************
 *
 */
inline
void
RawBuffer::writeInt16(boost::int16_t value, std::vector<boost::uint8_t>&buffer, std::size_t& index)
{
	buffer[index++] = static_cast<boost::uint8_t>(value >> 8);
	buffer[index++] = static_cast<boost::uint8_t>(value);
}

/*******************************************************************************
 *
 */
inline
boost::int16_t
RawBuffer::readInt16(const std::vector<boost::uint8_t>&buffer, std::size_t& index)
{
	union {
		boost::int16_t i;
		boost::uint16_t ui;
	};
	ui  = buffer[index++] << 8;
	ui += buffer[index++];
	return i;
}

/*******************************************************************************
 *
 */
inline
void
RawBuffer::reset()
{
	readIndex_ = 0;
	buffer_.resize(0);
}

/*******************************************************************************
 *
 */
inline
void
RawBuffer::reserve(std::size_t size)
{
	readIndex_ = 0;
	buffer_.resize(size);
}

/*******************************************************************************
 *
 */
inline
void
RawBuffer::putInt16(boost::int16_t value)
{
	std::size_t endIndex = buffer_.size();
	buffer_.resize(endIndex + sizeof(value));
	writeInt16(value, buffer_, endIndex);
}

/*******************************************************************************
 *
 */
inline
boost::int16_t
RawBuffer::getInt16()
{
	if (readIndex_ > buffer_.size() - sizeof(boost::int16_t)) {
		THROW_EXCEPTION(EndOfBufferException, "Could not get an int16 from the buffer.");
	}

	return readInt16(buffer_, readIndex_);
}

/*******************************************************************************
 *
 */
inline
void
RawBuffer::putUInt32(boost::uint32_t value)
{
	std::size_t endIndex = buffer_.size();
	buffer_.resize(endIndex + sizeof(value));
	writeUInt32(value, buffer_, endIndex);
}

/*******************************************************************************
 *
 */
inline
boost::uint32_t
RawBuffer::getUInt32()
{
	if (readIndex_ > buffer_.size() - sizeof(boost::uint32_t)) {
		THROW_EXCEPTION(EndOfBufferException, "Could not get a uint32 from the buffer.");
	}

	return readUInt32(buffer_, readIndex_);
}

/*******************************************************************************
 *
 */
inline
void
RawBuffer::putFloat(float value)
{
	union {
		boost::uint32_t i;
		float f;
	};
	f = value;
	std::size_t endIndex = buffer_.size();
	buffer_.resize(endIndex + sizeof(value));
	writeUInt32(i, buffer_, endIndex);
}

/*******************************************************************************
 *
 */
inline
float
RawBuffer::getFloat()
{
	if (readIndex_ > buffer_.size() - sizeof(float)) {
		THROW_EXCEPTION(EndOfBufferException, "Could not get a float from the buffer.");
	}

	union {
		boost::uint32_t i;
		float f;
	};
	i = readUInt32(buffer_, readIndex_);
	return f;
}

/*******************************************************************************
 *
 */
inline
void
RawBuffer::putString(const std::string& s)
{
	//TODO: check size (max uint32)
	std::size_t stringSize = s.size();
	putUInt32(stringSize);

	std::size_t endIndex = buffer_.size();
	buffer_.resize(endIndex + stringSize);
	memcpy(&buffer_[endIndex], &s[0], stringSize);
}

/*******************************************************************************
 *
 */
inline
void
RawBuffer::getString(std::string& s)
{
	boost::uint32_t stringSize = getUInt32();

	if (readIndex_ > buffer_.size() - stringSize) {
		THROW_EXCEPTION(EndOfBufferException, "Could not get the string from the buffer.");
	}

	s.resize(stringSize);
	memcpy(&s[0], &buffer_[readIndex_], stringSize);
	readIndex_ += stringSize;
}

/*******************************************************************************
 *
 */
inline
void
RawBuffer::putFloatArray(const std::vector<float>& a)
{
	//TODO: check size (max uint32)
	std::size_t arraySize = a.size();
	putUInt32(arraySize);

	union {
		boost::uint32_t i;
		float f;
	};
	std::size_t endIndex = buffer_.size();
	buffer_.resize(endIndex + arraySize * sizeof(boost::uint32_t));
	for (boost::uint32_t j = 0; j < arraySize; ++j) {
		f = a[j];
		writeUInt32(i, buffer_, endIndex);
	}
}

/*******************************************************************************
 *
 */
inline
void
RawBuffer::getFloatArray(std::vector<float>& a)
{
	boost::uint32_t arraySize = getUInt32();

	if (readIndex_ > buffer_.size() - arraySize * sizeof(float)) {
		THROW_EXCEPTION(EndOfBufferException, "Could not get the float array from the buffer.");
	}

	a.resize(arraySize);
	union {
		boost::uint32_t i;
		float f;
	};
	for (boost::uint32_t j = 0; j < arraySize; ++j) {
		i = readUInt32(buffer_, readIndex_);
		a[j] = f;
	}
}

/*******************************************************************************
 *
 */
template<typename T>
void
RawBuffer::putInt16Array(const std::vector<T>& a)
{
	putInt16Array(&a[0], a.size());
}

/*******************************************************************************
 *
 */
template<typename T>
void
RawBuffer::putInt16Array(const T* a, std::size_t arraySize)
{
	//TODO: check range???
	//TODO: check size (max uint32)
	putUInt32(arraySize);

	std::size_t endIndex = buffer_.size();
	buffer_.resize(endIndex + arraySize * sizeof(boost::int16_t));
	for (boost::uint32_t j = 0; j < arraySize; ++j) {
		writeInt16(static_cast<boost::int16_t>(a[j]), buffer_, endIndex);
	}
}

/*******************************************************************************
 *
 */
template<typename T>
void
RawBuffer::getInt16Array(std::vector<T>& a)
{
	boost::uint32_t arraySize = getUInt32();

	if (readIndex_ > buffer_.size() - arraySize * sizeof(boost::int16_t)) {
		THROW_EXCEPTION(EndOfBufferException, "Could not get the int16 array from the buffer.");
	}

	a.resize(arraySize);
	for (boost::uint32_t j = 0; j < arraySize; ++j) {
		a[j] = static_cast<T>(readInt16(buffer_, readIndex_));
	}
}

/*******************************************************************************
 *
 */
template<typename T>
void
RawBuffer::getInt16Array(T* a, std::size_t arraySize)
{
	boost::uint32_t receivedArraySize = getUInt32();
	if (receivedArraySize != arraySize) {
		THROW_EXCEPTION(WrongBufferSizeException, "Wrong buffer size. Received=" << receivedArraySize << ", correct=" << arraySize << '.');
	}

	if (readIndex_ > buffer_.size() - arraySize * sizeof(boost::int16_t)) {
		THROW_EXCEPTION(EndOfBufferException, "Could not get the int16 array from the buffer.");
	}

	for (boost::uint32_t j = 0; j < arraySize; ++j) {
		a[j] = static_cast<T>(readInt16(buffer_, readIndex_));
	}
}

} // namespace Lab

#endif /* RAWBUFFER_H_ */
