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

#ifndef ARRAYACQSERVER_H_
#define ARRAYACQSERVER_H_

#include <iostream>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>

#include "ArrayAcqServerProtocol.h"
#include "Exception.h"



namespace Lab {

struct BindException : public virtual Exception {};

/*******************************************************************************
 *
 */
template<typename AcqDevice>
class ArrayAcqServer {
public:
	ArrayAcqServer(unsigned short portNumber, AcqDevice& acqDevice);
	~ArrayAcqServer();

	void exec();
	void stop();
private:
	ArrayAcqServer(const ArrayAcqServer&);
	ArrayAcqServer& operator=(const ArrayAcqServer&);

	void handleAccept(const boost::system::error_code& error);

	boost::asio::io_service ioService_;
	boost::asio::ip::tcp::acceptor acceptor_;
	boost::asio::ip::tcp::socket socket_;
	AcqDevice& acqDevice_;
	ArrayAcqServerProtocol<AcqDevice> protocol_;
};

/*******************************************************************************
 * Constructor.
 */
template<typename AcqDevice>
ArrayAcqServer<AcqDevice>::ArrayAcqServer(unsigned short portNumber, AcqDevice& acqDevice)
		: ioService_()
		, acceptor_(ioService_)
		, socket_(ioService_)
		, acqDevice_(acqDevice)
		, protocol_(acqDevice_)
{
	boost::asio::ip::tcp::endpoint endPoint(boost::asio::ip::tcp::v4(), portNumber);
	acceptor_.open(endPoint.protocol());

	boost::system::error_code ec;
	acceptor_.bind(endPoint, ec);
	if (ec) THROW_EXCEPTION(BindException, "Bind error: " << ec.message());

	acceptor_.listen();

	boost::asio::ip::tcp::no_delay option(true);
	acceptor_.set_option(option);
}

/*******************************************************************************
 * Destructor.
 */
template<typename AcqDevice>
ArrayAcqServer<AcqDevice>::~ArrayAcqServer()
{
}

/*******************************************************************************
 *
 */
template<typename AcqDevice>
void
ArrayAcqServer<AcqDevice>::exec()
{
	std::cout << "Before accept" << std::endl;

//	boost::system::error_code ec;
//	acceptor_.accept(socket_, ec);
//	handleAccept(ec);

	acceptor_.async_accept(
			socket_,
			boost::bind(&ArrayAcqServer<AcqDevice>::handleAccept, this, boost::asio::placeholders::error));
	ioService_.run();
}

/*******************************************************************************
 *
 */
template<typename AcqDevice>
void
ArrayAcqServer<AcqDevice>::handleAccept(const boost::system::error_code& ec)
{
	if (ec) {
		THROW_EXCEPTION(NetworkException, "Error in accept: " << ec.message());
	} else {
		std::cout << "Before protocol_.exec" << std::endl;
		protocol_.exec(socket_);
		std::cout << "After protocol_.exec" << std::endl;
	}
}

/*******************************************************************************
 *
 */
template<typename AcqDevice>
void
ArrayAcqServer<AcqDevice>::stop()
{
	ioService_.stop();
}

} // namespace Lab

#endif /* ARRAYACQSERVER_H_ */
