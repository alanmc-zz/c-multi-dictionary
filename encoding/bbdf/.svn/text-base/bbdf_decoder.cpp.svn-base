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
/*
  Code in this file is partially based on code
  from the Micro-Hessian Java implementation.
*/
#include "encoding/common/wrappers.h"
#include "encoding/bbdf/bbdf_decoder.h"

using namespace BBDF::wrappers;

namespace BBDF {
    
  void bbdf_decoder::initialize() throw(io_exception) {
    int tag = is_->read();
    if (tag != 'm')
      throw expect("bbdf message", tag);
    
  }
  
  void bbdf_decoder::finalize() throw(io_exception) {
    int tag = is_->read();
    if (tag != 'z')
      throw expect("end of reply", tag);
  }
  
  std::hash_map<std::string, boost::shared_ptr<Object> > bbdf_decoder::decode() throw(io_exception) {
    boost::shared_ptr<Map> map = boost::dynamic_pointer_cast<Map>(read_object());
    
    if ( !map ) {
      throw io_exception("Bad args");
    }
    
    finalize();
    
    return map->value();
  }

  boost::shared_ptr<Object> bbdf_decoder::read_object() throw(io_exception) {
      int tag = is_->read();
      return read_object(tag);
    }

  boost::shared_ptr<Object> bbdf_decoder::read_object(int tag) throw(io_exception) {
    
    switch (tag) {
    case 'b':
    case 'B':
      { 
      boost::shared_ptr<Object> binary(new Binary(read_bytes(tag)));
      return binary;
      }
    case 'T':
    case 'F': 
      {
      boost::shared_ptr<Object> boolean(new Boolean(read_boolean(tag)));
      return boolean;
      }
    case 'd': 
      {
      boost::shared_ptr<Object> date(new Date(read_date(tag)));
      return date;
      }
    case 'D': 
      {
	boost::shared_ptr<Object> d(new Double(read_double(tag)));
	return d;
      }
    case 'f': 
      {
      boost::shared_ptr<Object> fault(new Fault(read_fault(tag)));
      return fault;
      }
    case 'I': 
      {
      boost::shared_ptr<Object> integer(new Integer(read_int(tag)));
      return integer;
      }
    case 'U': 
      {
      boost::shared_ptr<Object> uinteger(new UnsignedInteger(read_unsigned_int(tag)));
      return uinteger;
      }
    case 'V': 
      {
	boost::shared_ptr<Object> v(new Vector(read_vector(tag)));
	return v;
      }
    case 'L': 
      {
      boost::shared_ptr<Object> l(new Long(read_long(tag)));
      return l;
      }
    case 'M': 
      {
      boost::shared_ptr<Object> m(new Map(read_map(tag)));
      return m;
      }
    case 'N': 
      {
      boost::shared_ptr<Object> n(new Null());
      return n;
      }
    case 'R': 
      {
      boost::shared_ptr<Object> ref(new Ref(read_ref(tag)));
      return ref;
      }
    case 'S': 
      {
      boost::shared_ptr<Object> str(new String(read_string(tag)));
      return str;
      }
    case 'X': 
      {
      boost::shared_ptr<Object> xml(new Xml(read_xml(tag)));
      return xml;
      }
    default:
      throw io_exception(string("bbdf_decoder::readObject(): tag ").append(1, (char) tag).append(" cannot be handled"));
	}
    }

  int bbdf_decoder::read_ref(int tag) throw(io_exception) {
    if (tag != 'R') {
      throw expect("ref", tag);
    }
    int b32 = is_->read() & 0xFF;
    int b24 = is_->read() & 0xFF;
    int b16 = is_->read() & 0xFF;
    int b8 = is_->read() & 0xFF;

    return((b32 << 24) + (b24 << 16) + (b16 << 8) + b8) & 0x00000000FFFFFFFF;
  }



    bool bbdf_decoder::read_boolean(int tag) throw(io_exception) {
	switch (tag) {
	case 'T': return true;
	case 'F': return false;
	default:
	    throw expect("bool", tag);
	}
    }

    int bbdf_decoder::read_int(int tag) throw(io_exception) {
	if (tag != 'I')
	    throw expect("integer", tag);

	int b32 = is_->read() & 0xFF;
	int b24 = is_->read() & 0xFF;
	int b16 = is_->read() & 0xFF;
	int b8 = is_->read() & 0xFF;

	return((b32 << 24) + (b24 << 16) + (b16 << 8) + b8) & 0x00000000FFFFFFFF;
    }

    uint32_t bbdf_decoder::read_unsigned_int(int tag) throw(io_exception) {
	if (tag != 'U')
	    throw expect("unsigned integer", tag);

	uint32_t b32 = is_->read() & 0xFF;
	uint32_t b24 = is_->read() & 0xFF;
	uint32_t b16 = is_->read() & 0xFF;
	uint32_t b8 = is_->read() & 0xFF;

	return((b32 << 24) + (b24 << 16) + (b16 << 8) + b8) & 0x00000000FFFFFFFF;
    }

    long long bbdf_decoder::read_long(int tag) throw(io_exception) {
	if (tag != 'L')
	    throw expect("long", tag);

	return read_long64();
    }

    double bbdf_decoder::read_double(int tag) throw(io_exception) {
	if (tag != 'D')
	    throw expect("double", tag);
	double lval = read_long64();
	double* dval = (double*) & lval;
	return *dval;
    }

    long long bbdf_decoder::read_date(int tag) throw(io_exception) {
	if (tag != 'd')
	    throw expect("date", tag);
	return read_long64();
    }

    long long bbdf_decoder::read_long64() throw(io_exception) {
	unsigned long long b64 = is_->read() & 0xFFULL;
	unsigned long long b56 = is_->read() & 0xFF;
	unsigned long long b48 = is_->read() & 0xFF;
	unsigned long long b40 = is_->read() & 0xFF;
	unsigned long long b32 = is_->read() & 0xFF;
	unsigned long long b24 = is_->read() & 0xFF;
	unsigned long long b16 = is_->read() & 0xFF;
	unsigned long long b8 = is_->read() & 0xFF;
	long long value = (b64 << 56) +
	    (b56 << 48) +
	    (b48 << 40) +
	    (b40 << 32) +
	    (b32 << 24) +
	    (b24 << 16) +
	    (b16 << 8) +
	    b8;
	return value;
    }

    string bbdf_decoder::read_string(int tag) throw(io_exception) {
	if (tag == 'N')
	    return string("");
	if (tag != 'S')
	    throw expect("string", tag);

	int b32 = is_->read() & 0xFF;
        int b24 = is_->read() & 0xFF;
        int b16 = is_->read() & 0xFF;
        int b8 = is_->read() & 0xFF;

        int32_t len = (((b32 << 24) + (b24 << 16) + (b16 << 8) + b8) & 0x00000000FFFFFFFF);

  //	int b16 = is_->read();
  //	int b8 = is_->read();
  //	int len = (b16 << 8) + b8;

	return read_string_impl(len);
    }

  void read_byte_chunk(boost::shared_ptr<input_stream> is, string& bytes) {
	int b16 = is->read() & 0xFF;
	int b8 = is->read() & 0xFF;
	int len = (b16 << 8) + b8;
	for (int i = 0; i < len; i++)
	  bytes.push_back( (char)is->read() );
    }

    string bbdf_decoder::read_bytes(int tag) throw(io_exception) {
	if (tag == 'N')
	    return("");
	string bos;
	while (tag == 'b') {
	    read_byte_chunk(is_, bos);
	    tag = is_->read();
	}

	if (tag != 'B')
	    throw expect("bytes", tag);
	read_byte_chunk(is_, bos);

	return bos;
    }

    string bbdf_decoder::read_xml(int tag) throw(io_exception) {
	if (tag == 'N') {
	    return("");
	}
	if (tag != 'X') {
	    throw expect("xml", tag);
	}
	int b16 = is_->read();
	int b8 = is_->read();

	int len = (b16 << 8) + b8;

	return read_string_impl(len);
    }

  std::vector<boost::shared_ptr<Object> > bbdf_decoder::read_vector(int tag) throw(io_exception) {
	if (tag == 'N') {
	  vector<boost::shared_ptr<Object> > l;
	  return l;
	}
	
	if (tag != 'V') {
	    throw expect("vector", tag);
	}
	int vector_length = 0;
	int meta = is_->read();

	// parse optional (meta) type and/or length
	while (meta == 't' || meta == 'l') {
	    if (meta == 't') {
		// type
		string vector_type = read_string('S');
	    }
	    if (meta == 'l') {
		// length
		int b32 = is_->read();
		int b24 = is_->read();
		int b16 = is_->read();
		int b8 = is_->read();
		vector_length = (b32 << 24) + (b24 << 16) + (b16 << 8) + b8;
	    }
	    meta = is_->read();
	}
	// read in the vector data
	std::vector<boost::shared_ptr<Object> > l;
	while (meta != 'z') {	// vector ends with 'z'
	  // read object
	  boost::shared_ptr<Object> obj = read_object(meta);
	  // add to vector
	  l.push_back(obj);
	  // next please
	  meta = is_->read();
	}
	return l;
    }

  std::hash_map<string,boost::shared_ptr<Object> > bbdf_decoder::read_map(int tag) throw(io_exception) {
    if (tag == 'N') {
      Map::basic_type l;
      return l;
    }
    
    if (tag != 'M') {
      throw expect("map", tag);
    }
  
    int meta = is_->read();
	// parse optional (meta) type
	//	while (meta == 't') {
	//	    if (meta == 't') {
		// type
	//		string map_type = read_string('S');
	//	    }
	//	    meta = is_->read();
	//	}
	// read in the map data
    Map::basic_type m;
    while (meta != 'z') {	// map ends with 'z'
      // read key object
      string key(read_string(meta));
      // read value object
      meta = is_->read();
      boost::shared_ptr<Object> val = read_object(meta);
      // add to map
      m.insert(std::make_pair(key, val));
      // next please
      meta = is_->read();
    }
    return m;
  }

  string bbdf_decoder::read_string_impl(int32_t length) throw(io_exception) {
    string sb;
    
    for (int32_t i = 0; i < length; i++) {
      int32_t ch = is_->read();
      sb.append(1, (char) ch);
    
      //if (ch < 0x80)
      //sb.append(1, (char) ch);
      //else if ((ch & 0xe0) == 0xc0) {
      //int ch1 = is_->read();
      //int v = ((ch & 0x1f) << 6) + (ch1 & 0x3f);
      //
      //sb.append(1, (char) v);
      //}
      //else if ((ch & 0xf0) == 0xe0) {
      //int ch1 = is_->read();
      //int ch2 = is_->read();
      //int v = ((ch & 0x0f) << 12) + ((ch1 & 0x3f) << 6) + (ch2 & 0x3f);
      //
      //sb.append(1, (char) v);
      //}
      //else
      //throw	io_exception("bad utf-8 encoding");
    }
    
    return sb;
  }
  
    Fault bbdf_decoder::read_fault(int tag) throw(io_exception) {
	if (tag != 'f')
	    throw expect("fault", tag);

	// skip over "code"
	int _tag = is_->read();
	read_string(_tag);
	// read code value
	_tag = is_->read();
	String code(read_string(_tag));

	// skip over "message"
	_tag = is_->read();
	read_string(_tag);
	// read message value
	_tag = is_->read();
	String message(read_string(_tag));

	// skip over "detail"
	_tag = is_->read();
	read_string(_tag);

	//read stack trace
	_tag = is_->read();
	Map destroyer(read_map(_tag));
	// the Map above acts goes out of scope below
	// and deletes itself to prevent a lot of leaks

	string mesg = message.value();
	// hack: exception type encoded in the message
	unsigned long exception_type_start = mesg.find(']');
	if (exception_type_start != string::npos) {
	    string exception_type(mesg.substr(0, exception_type_start));
	    string exception_mesg(mesg.substr(exception_type_start + 1));
	    return Fault(exception_type, exception_mesg);
	}
	// end hack
	return Fault(code, message);
    }

    io_exception bbdf_decoder::expect(const string& expect, int ch) {
	if (ch < 0)
	    return io_exception("expected " + expect + " at end of file");
	else
	    return io_exception("expected " + expect + " at " + (char) ch);
    }
}

