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

#ifndef TESTDEVICE_H_
#define TESTDEVICE_H_

#include <string>
#include <vector>

#include <boost/cstdint.hpp>

#include "Exception.h"
#include "Matrix.h"



namespace Lab {

struct TestDeviceException : public virtual Exception {};

class TestDevice {
public:
	TestDevice(const std::string& dataFile, const std::string& datasetName);
	~TestDevice();

	const std::vector<boost::int16_t>& getSignal();
	boost::uint32_t getSignalLength() const;
	boost::int16_t getMaxSampleValue() const;
	boost::int16_t getMinSampleValue() const;
	float getSamplingFrequency() const;

	void setActiveReceiveElements(const std::string& mask /* '0', '1'*/);
	void setActiveTransmitElements(const std::string& mask /* '0', '1'*/);
	void setAcquisitionTime(float acqTime /* s */);
	void setBaseElement(unsigned short baseElement /* 0 ... (NUM_CHANNELS_MUX - NUM_CHANNELS)*/);
	void setCenterFrequency(float fc /* Hz */, int numPulses);
	void setGain(float gain /* 0.0 ... 60.0 dB */);
	void setReceiveDelays(const std::vector<float>& delays /* s */);
	void setSamplingFrequency(float fs /* Hz */);
	void setTransmitDelays(const std::vector<float>& delays /* s */);

	void execPreConfiguration();
	void execPostConfiguration();
	void execPreLoopConfiguration();
	void execPostLoopConfiguration();

private:
	TestDevice(const TestDevice&) = delete;
	TestDevice& operator=(const TestDevice&) = delete;

	unsigned int numActiveRxElem_;
	unsigned int signalLength_;
	float fs_;
	Matrix<float> rawData_;
	std::vector<boost::int16_t> signalBuffer_;
};

} // namespace Lab

#endif /* TESTDEVICE_H_ */
