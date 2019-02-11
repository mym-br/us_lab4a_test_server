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

#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <exception>
#include <sstream>
#include <string>



namespace Lab {

#define THROW_EXCEPTION(E,M) \
	do {\
		ErrorMessage em;\
		E exc;\
		try { em << M << " [[[file: " << __FILE__ << " line: " << __LINE__ << "]]]"; } catch (...) {}\
		exc.setMessage(em);\
		throw exc;\
	} while (false)

/*******************************************************************************
 *
 *
 * This class may throw std::bad_alloc.
 */
class ErrorMessage {
public:
	ErrorMessage() {}
	~ErrorMessage() {}

	template<typename T>
	ErrorMessage& operator<<(const T& messagePart)
	{
		buffer_ << messagePart;
		return *this;
	}

	ErrorMessage& operator<<(const std::exception& e)
	{
		buffer_ << e.what();
		return *this;
	}

	std::string getString() const
	{
		return buffer_.str();
	}
private:
	ErrorMessage(const ErrorMessage&);
	ErrorMessage& operator=(const ErrorMessage&);

	std::ostringstream buffer_;
};

/*******************************************************************************
 *
 */
struct Exception : public virtual std::exception {
	Exception() throw() {}
	~Exception() throw() {}

	virtual const char* what() const throw()
	{
		const char* cs = "";
		try {
			cs = message.c_str();
		} catch (...) {
			// Ignore.
		}
		return cs;
	}

	void setMessage(const ErrorMessage& em)
	{
		try {
			message = em.getString();
		} catch (...) {
			// Ignore.
		}
	}

	std::string message;
};

struct EndOfBufferException : public virtual Exception {};
struct InvalidCallException : public virtual Exception {};
struct InvalidFileException : public virtual Exception {};
struct InvalidParameterException : public virtual Exception {};
struct InvalidStateException : public virtual Exception {};
struct InvalidValueException : public virtual Exception {};
struct IOException : public virtual Exception {};
struct MissingValueException : public virtual Exception {};
struct NetworkException : public virtual Exception {};
struct UnavailableResourceException : public virtual Exception {};
struct WrongBufferSizeException : public virtual Exception {};

} // namespace Lab

#endif /* EXCEPTION_H_ */
