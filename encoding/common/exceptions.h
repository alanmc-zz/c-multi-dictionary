// Copyright (C) 2004 Aldratech Ltd. See the LICENSE file for licensing information.
/*
 This file is part of hessiancpp.

    hessiancpp is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.

    hessiancpp is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with hessiancpp; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef BBDF_EXCEPTIONS_H
#define BBDF_EXCEPTIONS_H

#include <exception>
#include <string>
using std::string;
using std::exception;

#include "encoding/common/wrappers.h"
using namespace BBDF::wrappers;

#ifdef WIN32
#pragma warning(disable:4290)
#endif

namespace BBDF {
    namespace exceptions {

	///io_exception class.
	class io_exception : public std::exception {
	public:
	    ///Constructor with error message.
	    explicit io_exception(const string& what): _message(what) {
	    }
	    ///Destructor.
	    virtual ~io_exception() throw() {
	    }
	    ///Get error message.
	    virtual const char* what() const throw() {
		return _message.c_str();
	    }
	    ///Re-throw.
	    virtual void raise() const {
		throw *this;
	    }
	private:
	    string _message;
	};

	///fault_exception class representing a Hessian Fault.
	class fault_exception : public std::exception {
	public:
	    ///Constructor with code and message.
	fault_exception(const string& code, const string& message): _code(code), _message(message) {
		_what = string("code ").append(_code).append(", message ").append(_message);
	    }
	    ///Constructor from Fault.
	fault_exception(const Fault& fault) : _code(fault.code()), _message(fault.message()) {
		_what = string("code ").append(_code).append(", message ").append(_message);
	    }
	    ///Destructor.
	    virtual ~fault_exception() throw() {
	    }
	    ///Get error message with code and message.
	    virtual const char* what() const throw() {
		return _what.c_str(); 
	    }
	    ///Re-throw.
	    virtual void raise() const {
		throw *this;
	    }
	    ///Get the Fault code.
	    string code() const throw() {
		return _code;
	    }
	    ///Get the Fault message.
	    string message() const throw() {
		return _message;
	    }
	private:
	    string _code;
	    string _message;
	    string _what;
	};
    }
}

#endif
