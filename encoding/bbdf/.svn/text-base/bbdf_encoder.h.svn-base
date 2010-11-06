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

#ifndef BBDF_OUTPUT_H
#define BBDF_OUTPUT_H

#include "encoding/common/exceptions.h"
#include "encoding/common/wrappers.h"
#include "boost/shared_ptr.hpp"

#include <string>

using namespace BBDF::exceptions;
using namespace BBDF::wrappers;
using namespace std;

namespace BBDF {
        ///Creates output serialisation for Hessian requests.
	class bbdf_encoder {
	public:
		///Constructor.
	        bbdf_encoder();
		
		///Destructor.
		~bbdf_encoder();
		
		bytes_t encode(boost::shared_ptr<Map> payload);		
		
	protected:
		///Write a Binary to the call.
		void write_binary(boost::shared_ptr<Binary> object);
		
		///Write a Boolean to the call.
		void write_boolean(boost::shared_ptr<Boolean> object);

		///Write a Date to the call.
		void write_date(boost::shared_ptr<Date> object);

		///Write a Double to the call.
		void write_double(boost::shared_ptr<Double> object);

		///Write a 32 bit Integer to the call.
		void write_integer(boost::shared_ptr<Integer> object);

		///Write a 32 bit UnsignedInteger to the call.
		void write_unsigned_integer(boost::shared_ptr<UnsignedInteger> object);

		///Write a Vector to the call.
		void write_vector(boost::shared_ptr<Vector> object);
		
		///Write a 64 bit Long to the call.
		void write_long(boost::shared_ptr<Long> object);
		
		///Write a Map to the call.
		void write_map(boost::shared_ptr<Map> object);
		
		///Write a Null to the call.
		void write_null();

		///Write an Object to the call.
		void write_object(boost::shared_ptr<Object> object);

		///Write a String to the call.
		void write_string(boost::shared_ptr<String> object);

		///Write an Xml to the call.
		void write_xml(boost::shared_ptr<Xml> object);

	private:
		///Start a Hessian method call.
		void initialize();
		
		///Finish a Hessian method call.
		void finalize();

		boost::shared_ptr<std::string> data_;

		void write_long64(long long value);
		void write_string(const string& value);
	};
}

#endif
