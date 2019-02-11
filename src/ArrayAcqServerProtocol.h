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

#ifndef ARRAYACQSERVERPROTOCOL_H_
#define ARRAYACQSERVERPROTOCOL_H_

#include <boost/asio/ip/tcp.hpp>

#include "ArrayAcqProtocol.h"
#include "Log.h"



namespace Lab {

template<typename AcqDevice>
class ArrayAcqServerProtocol : private ArrayAcqProtocol {
public:
	ArrayAcqServerProtocol(AcqDevice& acqDevice) : acqDevice_(acqDevice) {}
	~ArrayAcqServerProtocol() {}

	void exec(boost::asio::ip::tcp::socket& socket);
private:
	ArrayAcqServerProtocol(const ArrayAcqServerProtocol&);
	ArrayAcqServerProtocol& operator=(const ArrayAcqServerProtocol&);

	void sendErrorResponse(const std::exception& e, boost::asio::ip::tcp::socket& socket);

	void handleConnectRequest(boost::asio::ip::tcp::socket& socket);

	void handleGetSignalLengthRequest(boost::asio::ip::tcp::socket& socket);
	void handleGetSignalRequest(boost::asio::ip::tcp::socket& socket);
	void handleGetMaxSampleValueRequest(boost::asio::ip::tcp::socket& socket);
	void handleGetMinSampleValueRequest(boost::asio::ip::tcp::socket& socket);
	void handleGetSamplingFrequencyRequest(boost::asio::ip::tcp::socket& socket);

	void handleSetAcquisitionTimeRequest(boost::asio::ip::tcp::socket& socket);
	void handleSetActiveReceiveElementsRequest(boost::asio::ip::tcp::socket& socket);
	void handleSetActiveTransmitElementsRequest(boost::asio::ip::tcp::socket& socket);
	void handleSetBaseElementRequest(boost::asio::ip::tcp::socket& socket);
	void handleSetCenterFrequencyRequest(boost::asio::ip::tcp::socket& socket);
	void handleSetGainRequest(boost::asio::ip::tcp::socket& socket);
	void handleSetReceiveDelaysRequest(boost::asio::ip::tcp::socket& socket);
	void handleSetSamplingFrequencyRequest(boost::asio::ip::tcp::socket& socket);
	void handleSetTransmitDelaysRequest(boost::asio::ip::tcp::socket& socket);

	void handleExecPreConfigurationRequest(boost::asio::ip::tcp::socket& socket);
	void handleExecPostConfigurationRequest(boost::asio::ip::tcp::socket& socket);
	void handleExecPreLoopConfigurationRequest(boost::asio::ip::tcp::socket& socket);
	void handleExecPostLoopConfigurationRequest(boost::asio::ip::tcp::socket& socket);

	AcqDevice& acqDevice_;
};

template<typename AcqDevice>
void
ArrayAcqServerProtocol<AcqDevice>::exec(boost::asio::ip::tcp::socket& socket)
{
	for (;;) {
		boost::uint32_t messageType = receiveMessage(socket);
		switch (messageType) {
		case CONNECT_REQUEST:
			handleConnectRequest(socket);
			LOG_DEBUG << "CONNECT_REQUEST";
			break;
		case DISCONNECT_REQUEST:
			LOG_DEBUG << "DISCONNECT_REQUEST";
			return;

		case GET_SIGNAL_LENGTH_REQUEST:
			handleGetSignalLengthRequest(socket);
			//LOG_DEBUG << "GET_SIGNAL_LENGTH_REQUEST";
			break;
		case GET_SIGNAL_REQUEST:
			handleGetSignalRequest(socket);
			//LOG_DEBUG << "GET_SIGNAL_REQUEST";
			break;
		case GET_MAX_SAMPLE_VALUE_REQUEST:
			handleGetMaxSampleValueRequest(socket);
			//LOG_DEBUG << "GET_MAX_SAMPLE_VALUE_REQUEST";
			break;
		case GET_MIN_SAMPLE_VALUE_REQUEST:
			handleGetMinSampleValueRequest(socket);
			//LOG_DEBUG << "GET_MIN_SAMPLE_VALUE_REQUEST";
			break;
		case GET_SAMPLING_FREQUENCY_REQUEST:
			handleGetSamplingFrequencyRequest(socket);
			//LOG_DEBUG << "GET_SAMPLING_FREQUENCY_REQUEST";
			break;

		case SET_ACQUISITION_TIME_REQUEST:
			handleSetAcquisitionTimeRequest(socket);
			//LOG_DEBUG << "SET_ACQUISITION_TIME_REQUEST";
			break;
		case SET_ACTIVE_RECEIVE_ELEMENTS_REQUEST:
			handleSetActiveReceiveElementsRequest(socket);
			//LOG_DEBUG << "SET_ACTIVE_RECEIVE_ELEMENTS_REQUEST";
			break;
		case SET_ACTIVE_TRANSMIT_ELEMENTS_REQUEST:
			handleSetActiveTransmitElementsRequest(socket);
			//LOG_DEBUG << "SET_ACTIVE_TRANSMIT_ELEMENTS_REQUEST";
			break;
		case SET_BASE_ELEMENT_REQUEST:
			handleSetBaseElementRequest(socket);
			//LOG_DEBUG << "SET_BASE_ELEMENT_REQUEST";
			break;
		case SET_CENTER_FREQUENCY_REQUEST:
			handleSetCenterFrequencyRequest(socket);
			//LOG_DEBUG << "SET_CENTER_FREQUENCY_REQUEST";
			break;
		case SET_GAIN_REQUEST:
			handleSetGainRequest(socket);
			//LOG_DEBUG << "SET_GAIN_REQUEST";
			break;
		case SET_RECEIVE_DELAYS_REQUEST:
			handleSetReceiveDelaysRequest(socket);
			//LOG_DEBUG << "SET_RECEIVE_DELAYS_REQUEST";
			break;
		case SET_SAMPLING_FREQUENCY_REQUEST:
			handleSetSamplingFrequencyRequest(socket);
			//LOG_DEBUG << "SET_SAMPLING_FREQUENCY_REQUEST";
			break;
		case SET_TRANSMIT_DELAYS_REQUEST:
			handleSetTransmitDelaysRequest(socket);
			//LOG_DEBUG << "SET_TRANSMIT_DELAYS_REQUEST";
			break;

		case EXEC_PRE_CONFIGURATION_REQUEST:
			handleExecPreConfigurationRequest(socket);
			//LOG_DEBUG << "EXEC_PRE_CONFIGURATION_REQUEST";
			break;
		case EXEC_POST_CONFIGURATION_REQUEST:
			handleExecPostConfigurationRequest(socket);
			//LOG_DEBUG << "EXEC_POST_CONFIGURATION_REQUEST";
			break;
		case EXEC_PRE_LOOP_CONFIGURATION_REQUEST:
			handleExecPreLoopConfigurationRequest(socket);
			//LOG_DEBUG << "EXEC_PRE_LOOP_CONFIGURATION_REQUEST";
			break;
		case EXEC_POST_LOOP_CONFIGURATION_REQUEST:
			handleExecPostLoopConfigurationRequest(socket);
			//LOG_DEBUG << "EXEC_POST_LOOP_CONFIGURATION_REQUEST";
			break;
		default:
			THROW_EXCEPTION(InvalidRequestException, "Invalid request: " << messageType << '.');
		}
	}
}

template<typename AcqDevice>
void
ArrayAcqServerProtocol<AcqDevice>::sendErrorResponse(const std::exception& e, boost::asio::ip::tcp::socket& socket)
{
	prepareMessage(ERROR_RESPONSE);
	dataRawBuffer_.putString(e.what());
	sendMessage(socket);
}

template<typename AcqDevice>
void
ArrayAcqServerProtocol<AcqDevice>::handleConnectRequest(boost::asio::ip::tcp::socket& socket)
{
	const boost::uint32_t protocolVersion = dataRawBuffer_.getUInt32();
	if (protocolVersion == PROTOCOL_VERSION) {
		prepareMessage(OK_RESPONSE);
	} else {
		prepareMessage(ERROR_RESPONSE);
		dataRawBuffer_.putString("Invalid protocol version.");
	}
	sendMessage(socket);
}

template<typename AcqDevice>
void
ArrayAcqServerProtocol<AcqDevice>::handleGetSignalLengthRequest(boost::asio::ip::tcp::socket& socket)
{
	boost::uint32_t signalLength = 0;
	try {
		signalLength = acqDevice_.getSignalLength();
	} catch (std::exception& e) {
		sendErrorResponse(e, socket);
		return;
	}

	prepareMessage(GET_SIGNAL_LENGTH_RESPONSE);
	dataRawBuffer_.putUInt32(signalLength);
	sendMessage(socket);
}

template<typename AcqDevice>
void
ArrayAcqServerProtocol<AcqDevice>::handleGetSignalRequest(boost::asio::ip::tcp::socket& socket)
{
	//const std::vector<float>* dataBuffer = nullptr;
	const std::vector<boost::int16_t>* dataBuffer = nullptr;
	try {
		dataBuffer = &acqDevice_.getSignal();
	} catch (std::exception& e) {
		sendErrorResponse(e, socket);
		return;
	}

	prepareMessage(GET_SIGNAL_RESPONSE);
	//dataRawBuffer_.putFloatArray(*dataBuffer);
	dataRawBuffer_.putInt16Array(*dataBuffer);
	sendMessage(socket);
}

template<typename AcqDevice>
void
ArrayAcqServerProtocol<AcqDevice>::handleGetMaxSampleValueRequest(boost::asio::ip::tcp::socket& socket)
{
	boost::int16_t v;
	try {
		v = acqDevice_.getMaxSampleValue();
	} catch (std::exception& e) {
		sendErrorResponse(e, socket);
		return;
	}

	prepareMessage(GET_MAX_SAMPLE_VALUE_RESPONSE);
	dataRawBuffer_.putInt16(v);
	sendMessage(socket);
}

template<typename AcqDevice>
void
ArrayAcqServerProtocol<AcqDevice>::handleGetMinSampleValueRequest(boost::asio::ip::tcp::socket& socket)
{
	boost::int16_t v;
	try {
		v = acqDevice_.getMinSampleValue();
	} catch (std::exception& e) {
		sendErrorResponse(e, socket);
		return;
	}

	prepareMessage(GET_MIN_SAMPLE_VALUE_RESPONSE);
	dataRawBuffer_.putInt16(v);
	sendMessage(socket);
}

template<typename AcqDevice>
void
ArrayAcqServerProtocol<AcqDevice>::handleGetSamplingFrequencyRequest(boost::asio::ip::tcp::socket& socket)
{
	float fs;
	try {
		fs = acqDevice_.getSamplingFrequency();
	} catch (std::exception& e) {
		sendErrorResponse(e, socket);
		return;
	}

	prepareMessage(GET_SAMPLING_FREQUENCY_RESPONSE);
	dataRawBuffer_.putFloat(fs);
	sendMessage(socket);
}

template<typename AcqDevice>
void
ArrayAcqServerProtocol<AcqDevice>::handleSetAcquisitionTimeRequest(boost::asio::ip::tcp::socket& socket)
{
	const float acqTime = dataRawBuffer_.getFloat();

	try {
		acqDevice_.setAcquisitionTime(acqTime);
	} catch (std::exception& e) {
		sendErrorResponse(e, socket);
		return;
	}

	prepareMessage(OK_RESPONSE);
	sendMessage(socket);
}

template<typename AcqDevice>
void
ArrayAcqServerProtocol<AcqDevice>::handleSetActiveReceiveElementsRequest(boost::asio::ip::tcp::socket& socket)
{
	std::string mask;
	dataRawBuffer_.getString(mask);

	try {
		acqDevice_.setActiveReceiveElements(mask);
	} catch (std::exception& e) {
		sendErrorResponse(e, socket);
		return;
	}

	prepareMessage(OK_RESPONSE);
	sendMessage(socket);
}

template<typename AcqDevice>
void
ArrayAcqServerProtocol<AcqDevice>::handleSetActiveTransmitElementsRequest(boost::asio::ip::tcp::socket& socket)
{
	std::string mask;
	dataRawBuffer_.getString(mask);

	try {
		acqDevice_.setActiveTransmitElements(mask);
	} catch (std::exception& e) {
		sendErrorResponse(e, socket);
		return;
	}

	prepareMessage(OK_RESPONSE);
	sendMessage(socket);
}

template<typename AcqDevice>
void
ArrayAcqServerProtocol<AcqDevice>::handleSetBaseElementRequest(boost::asio::ip::tcp::socket& socket)
{
	const boost::uint32_t baseElement = dataRawBuffer_.getUInt32();

	try {
		acqDevice_.setBaseElement(baseElement);
	} catch (std::exception& e) {
		sendErrorResponse(e, socket);
		return;
	}

	prepareMessage(OK_RESPONSE);
	sendMessage(socket);
}

template<typename AcqDevice>
void
ArrayAcqServerProtocol<AcqDevice>::handleSetCenterFrequencyRequest(boost::asio::ip::tcp::socket& socket)
{
	const float fc = dataRawBuffer_.getFloat();
	const int numPulses = static_cast<int>(dataRawBuffer_.getUInt32());

	try {
		acqDevice_.setCenterFrequency(fc, numPulses);
	} catch (std::exception& e) {
		sendErrorResponse(e, socket);
		return;
	}

	prepareMessage(OK_RESPONSE);
	sendMessage(socket);
}

template<typename AcqDevice>
void
ArrayAcqServerProtocol<AcqDevice>::handleSetGainRequest(boost::asio::ip::tcp::socket& socket)
{
	const float gain = dataRawBuffer_.getFloat();

	try {
		acqDevice_.setGain(gain);
	} catch (std::exception& e) {
		sendErrorResponse(e, socket);
		return;
	}

	prepareMessage(OK_RESPONSE);
	sendMessage(socket);
}

template<typename AcqDevice>
void
ArrayAcqServerProtocol<AcqDevice>::handleSetReceiveDelaysRequest(boost::asio::ip::tcp::socket& socket)
{
	std::vector<float> delays;
	dataRawBuffer_.getFloatArray(delays);

	try {
		acqDevice_.setReceiveDelays(delays);
	} catch (std::exception& e) {
		sendErrorResponse(e, socket);
		return;
	}

	prepareMessage(OK_RESPONSE);
	sendMessage(socket);
}

template<typename AcqDevice>
void
ArrayAcqServerProtocol<AcqDevice>::handleSetSamplingFrequencyRequest(boost::asio::ip::tcp::socket& socket)
{
	const float fs = dataRawBuffer_.getFloat();
	LOG_DEBUG << "fs = " << fs;

	try {
		acqDevice_.setSamplingFrequency(fs);
	} catch (std::exception& e) {
		sendErrorResponse(e, socket);
		return;
	}

	prepareMessage(OK_RESPONSE);
	sendMessage(socket);
}

template<typename AcqDevice>
void
ArrayAcqServerProtocol<AcqDevice>::handleSetTransmitDelaysRequest(boost::asio::ip::tcp::socket& socket)
{
	std::vector<float> delays;
	dataRawBuffer_.getFloatArray(delays);

	try {
		acqDevice_.setTransmitDelays(delays);
	} catch (std::exception& e) {
		sendErrorResponse(e, socket);
		return;
	}

	prepareMessage(OK_RESPONSE);
	sendMessage(socket);
}

template<typename AcqDevice>
void
ArrayAcqServerProtocol<AcqDevice>::handleExecPreConfigurationRequest(boost::asio::ip::tcp::socket& socket)
{
	try {
		acqDevice_.execPreConfiguration();
	} catch (std::exception& e) {
		sendErrorResponse(e, socket);
		return;
	}

	prepareMessage(OK_RESPONSE);
	sendMessage(socket);
}

template<typename AcqDevice>
void
ArrayAcqServerProtocol<AcqDevice>::handleExecPostConfigurationRequest(boost::asio::ip::tcp::socket& socket)
{
	try {
		acqDevice_.execPostConfiguration();
	} catch (std::exception& e) {
		sendErrorResponse(e, socket);
		return;
	}

	prepareMessage(OK_RESPONSE);
	sendMessage(socket);
}

template<typename AcqDevice>
void
ArrayAcqServerProtocol<AcqDevice>::handleExecPreLoopConfigurationRequest(boost::asio::ip::tcp::socket& socket)
{
	try {
		acqDevice_.execPreLoopConfiguration();
	} catch (std::exception& e) {
		sendErrorResponse(e, socket);
		return;
	}

	prepareMessage(OK_RESPONSE);
	sendMessage(socket);
}

template<typename AcqDevice>
void
ArrayAcqServerProtocol<AcqDevice>::handleExecPostLoopConfigurationRequest(boost::asio::ip::tcp::socket& socket)
{
	try {
		acqDevice_.execPostLoopConfiguration();
	} catch (std::exception& e) {
		sendErrorResponse(e, socket);
		return;
	}

	prepareMessage(OK_RESPONSE);
	sendMessage(socket);
}


} // namespace Lab

#endif /* ARRAYACQSERVERPROTOCOL_H_ */
