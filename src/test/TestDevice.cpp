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

#include "TestDevice.h"

#include <cmath>
#include <vector>

#include "HDF5Util.h"
#include "Log.h"
#include "Util.h"

#define PAUSE_AFTER_SIGNAL_ACQ_MS 1
#define SAMPLING_FREQUENCY_EPS (2e-7f)

#define MAX_SAMPLE_VALUE (2047)
#define MIN_SAMPLE_VALUE (-2048)
#define SCALE (0.5f)
#define NOISE_LEVEL (0.005f)



namespace Lab {

TestDevice::TestDevice(const std::string& dataFile, const std::string& datasetName)
		: numActiveRxElem_()
		, signalLength_()
		, fs_()
		, prngEngine_()
		, prngDist_(NOISE_LEVEL * MIN_SAMPLE_VALUE, NOISE_LEVEL * MAX_SAMPLE_VALUE)
{
	LOG_DEBUG << "TestDevice()";
	LOG_DEBUG << "dataFile=" << dataFile;
	LOG_DEBUG << "datasetName=" << datasetName;

	HDF5Util::load2(dataFile, datasetName, rawData_);
	numActiveRxElem_ = rawData_.n1();
	signalLength_ = rawData_.n2();
	Util::normalize(rawData_);
	const float factor = SCALE * MAX_SAMPLE_VALUE;
	Util::multiply(rawData_, factor);
	signalBuffer_.resize(rawData_.n1() * rawData_.n2());
}

TestDevice::~TestDevice()
{
}

const std::vector<boost::int16_t>&
TestDevice::getSignal()
{
	LOG_DEBUG << "getSignal()";

	auto srcIter = rawData_.begin();
	auto endSrcIter = rawData_.end();
	auto destIter = signalBuffer_.begin();
	while (srcIter != endSrcIter) {
		*destIter = static_cast<boost::int16_t>(std::round(*srcIter)) + prngDist_(prngEngine_);
		++srcIter;
		++destIter;
	}

	Util::sleepMs(PAUSE_AFTER_SIGNAL_ACQ_MS);

	return signalBuffer_;
}

boost::uint32_t
TestDevice::getSignalLength() const
{
	LOG_DEBUG << "getSignalLength(): " << signalLength_;
	return signalLength_;
}

boost::int16_t
TestDevice::getMaxSampleValue() const
{
	LOG_DEBUG << "getMaxSampleValue(): " << MAX_SAMPLE_VALUE;
	return MAX_SAMPLE_VALUE;
}

boost::int16_t
TestDevice::getMinSampleValue() const
{
	LOG_DEBUG << "getMinSampleValue(): " << MIN_SAMPLE_VALUE;
	return MIN_SAMPLE_VALUE;
}

float
TestDevice::getSamplingFrequency() const
{
	LOG_DEBUG << "getSamplingFrequency(): " << fs_;
	return fs_;
}

void
TestDevice::setActiveReceiveElements(const std::string& mask)
{
	LOG_DEBUG << "setActiveReceiveElements(): " << mask;
}

void
TestDevice::setActiveTransmitElements(const std::string& mask)
{
	LOG_DEBUG << "setActiveTransmitElements(): " << mask;
}

void
TestDevice::setAcquisitionTime(float acqTime)
{
	LOG_DEBUG << "setAcquisitionTime(): " << acqTime;
}

void
TestDevice::setBaseElement(unsigned short baseElement)
{
	LOG_DEBUG << "setBaseElement(): " << baseElement;
}

void
TestDevice::setCenterFrequency(float fc, int numPulses)
{
	LOG_DEBUG << "setCenterFrequency(): fc=" << fc << " numPulses=" << numPulses;
}

void
TestDevice::setGain(float gain)
{
	LOG_DEBUG << "setGain(): " << gain;
}

void
TestDevice::setReceiveDelays(const std::vector<float>& delays)
{
	LOG_DEBUG << "setReceiveDelays():";
	for (auto v : delays) {
		LOG_DEBUG << "    " << v;
	}
}

void
TestDevice::setSamplingFrequency(float fs)
{
	LOG_DEBUG << "setSamplingFrequency(): " << fs;
	fs_ = fs;
}

void
TestDevice::setTransmitDelays(const std::vector<float>& delays)
{
	LOG_DEBUG << "setTransmitDelays():";
	for (auto v : delays) {
		LOG_DEBUG << "    " << v;
	}
}

void
TestDevice::execPreConfiguration()
{
	LOG_DEBUG << "execPreConfiguration()";
}

void
TestDevice::execPostConfiguration()
{
	LOG_DEBUG << "execPostConfiguration()";
}

void
TestDevice::execPreLoopConfiguration()
{
	LOG_DEBUG << "execPreLoopConfiguration()";
}

void
TestDevice::execPostLoopConfiguration()
{
	LOG_DEBUG << "execPostLoopConfiguration()";
}

} // namespace Lab
