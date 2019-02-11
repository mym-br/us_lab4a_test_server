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

#ifndef ARRAYACQPROTOCOL_H_
#define ARRAYACQPROTOCOL_H_

#include "RawBuffer.h"

#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>
#include <boost/cstdint.hpp>

#include "Exception.h"



namespace Lab {

struct InvalidMessageTypeException : public virtual Exception {};
struct InvalidRequestException : public virtual Exception {};
struct ServerException : public virtual Exception {};

/*******************************************************************************
 *
 */
class ArrayAcqProtocol {
protected:
	enum {
		HEADER_RAW_BUFFER_SIZE = 8,
		PROTOCOL_VERSION = 1006
	};
	enum MessageType {
		CONNECT_REQUEST = 2001,
		DISCONNECT_REQUEST,

		OK_RESPONSE,
		ERROR_RESPONSE,

		GET_SIGNAL_LENGTH_REQUEST,
		GET_SIGNAL_LENGTH_RESPONSE,
		GET_SIGNAL_REQUEST,
		GET_SIGNAL_RESPONSE,
		GET_MAX_SAMPLE_VALUE_REQUEST,
		GET_MAX_SAMPLE_VALUE_RESPONSE,
		GET_MIN_SAMPLE_VALUE_REQUEST,
		GET_MIN_SAMPLE_VALUE_RESPONSE,
		GET_SAMPLING_FREQUENCY_REQUEST,
		GET_SAMPLING_FREQUENCY_RESPONSE,

		SET_ACQUISITION_TIME_REQUEST,
		SET_ACTIVE_RECEIVE_ELEMENTS_REQUEST,
		SET_ACTIVE_TRANSMIT_ELEMENTS_REQUEST,
		SET_BASE_ELEMENT_REQUEST,
		SET_CENTER_FREQUENCY_REQUEST,
		SET_GAIN_REQUEST,
		SET_RECEIVE_DELAYS_REQUEST,
		SET_SAMPLING_FREQUENCY_REQUEST,
		SET_TRANSMIT_DELAYS_REQUEST,

		EXEC_PRE_CONFIGURATION_REQUEST,
		EXEC_POST_CONFIGURATION_REQUEST,
		EXEC_PRE_LOOP_CONFIGURATION_REQUEST,
		EXEC_POST_LOOP_CONFIGURATION_REQUEST
	};

	ArrayAcqProtocol() {}
	~ArrayAcqProtocol() {}

	void prepareMessage(MessageType type);
	void sendMessage(boost::asio::ip::tcp::socket& socket);
	boost::uint32_t receiveMessage(boost::asio::ip::tcp::socket& socket);

	RawBuffer headerRawBuffer_;
	RawBuffer dataRawBuffer_;
private:
	ArrayAcqProtocol(const ArrayAcqProtocol&);
	ArrayAcqProtocol& operator=(const ArrayAcqProtocol&);
};

/*******************************************************************************
 *
 */
inline
void
ArrayAcqProtocol::prepareMessage(MessageType type)
{
	headerRawBuffer_.reset();
	headerRawBuffer_.putUInt32(type);

	dataRawBuffer_.reset();
}

/*******************************************************************************
 *
 */
inline
void
ArrayAcqProtocol::sendMessage(boost::asio::ip::tcp::socket& socket)
{
	headerRawBuffer_.putUInt32(dataRawBuffer_.size());

	std::size_t n = boost::asio::write(socket, boost::asio::buffer(&(headerRawBuffer_.front()), headerRawBuffer_.size()));
	if (n != headerRawBuffer_.size()) {
		THROW_EXCEPTION(IOException, "[ArrayAcqProtocol::sendMessage] Wrong number of header bytes sent: " << n << " (expected: " << headerRawBuffer_.size() << ").");
	}
	if (dataRawBuffer_.size() > 0) {
		n = boost::asio::write(socket, boost::asio::buffer(&dataRawBuffer_.front(), dataRawBuffer_.size()));
		if (n != dataRawBuffer_.size()) {
			THROW_EXCEPTION(IOException, "[ArrayAcqProtocol::sendMessage] Wrong number of data bytes sent: " << n << " (expected: " << dataRawBuffer_.size() << ").");
		}
	}
}

/*******************************************************************************
 *
 */
inline
boost::uint32_t
ArrayAcqProtocol::receiveMessage(boost::asio::ip::tcp::socket& socket)
{
	headerRawBuffer_.reserve(HEADER_RAW_BUFFER_SIZE);
	std::size_t n = boost::asio::read(socket, boost::asio::buffer(&headerRawBuffer_.front(), headerRawBuffer_.size()));
	if (n != headerRawBuffer_.size()) {
		THROW_EXCEPTION(IOException, "[ArrayAcqProtocol::receiveMessage] Wrong number of header bytes received: " << n << " (expected: " << headerRawBuffer_.size() << ").");
	}

	boost::uint32_t messageType = headerRawBuffer_.getUInt32();

	//TODO: check type???
	const boost::uint32_t dataSize = headerRawBuffer_.getUInt32();

	if (dataSize > 0) {
		dataRawBuffer_.reserve(dataSize);
		n = boost::asio::read(socket, boost::asio::buffer(&dataRawBuffer_.front(), dataRawBuffer_.size()));
		if (n != dataRawBuffer_.size()) {
			THROW_EXCEPTION(IOException, "[ArrayAcqProtocol::receiveMessage] Wrong number of data bytes received: " << n << " (expected: " << dataRawBuffer_.size() << ").");
		}
	}

	return messageType;
}

} // namespace Lab

#endif /* ARRAYACQPROTOCOL_H_ */
