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

#include "encoding/bbdf/bbdf_encoder.h"
#include "encoding/common/zlibdec.h"

using namespace BBDF::exceptions;
using namespace BBDF::wrappers;

namespace BBDF {
    
    bbdf_encoder::bbdf_encoder() :
      data_(new std::string()) {
      initialize();
    }
  
  bbdf_encoder::~bbdf_encoder() {
    //delete data_;
  }
  
  bytes_t bbdf_encoder::encode(boost::shared_ptr<Map> payload) {
    write_object(payload);
    finalize();
    
    //    zlibdec compressor(0);
    //    std::string compData = compressor.compress_string(*data_);

    //std::cout << "Data: " << *data_ << std::endl << std::endl;
    //std::cout << "Comp Data: " << compData << std::endl;
	      
    
    const char* bytesCpy = (const char*)malloc(data_->size());
    memcpy((void*)bytesCpy, (void*)data_->data(), data_->size());
    
    //const char* bytesCpy = (const char*)malloc(compData.size());
    //memcpy((void*)bytesCpy, (void*)compData.data(), compData.size());

    bytes_t b;
    
    b.bytes = bytesCpy;
    b.size = data_->size();
    //b.size = compData.size();
    
    return b;
  }
    
    void bbdf_encoder::initialize() {
	data_->append(1, 'm');
    }
    
    void bbdf_encoder::finalize() {
 	data_->append(1, 'z');
	data_->append(1, '\0');
    }
    
  void bbdf_encoder::write_binary(boost::shared_ptr<Binary> object) {			
	if (object == NULL) {						
	    write_null();				
	    return;
	}								
	const string& l_bytes = object->value();			
	size_t l_bytesLen = l_bytes.size();				
	size_t l_pos = 0;						
	while(l_bytesLen > 0x8000)					
	{								
	    size_t l_sublen = 0x8000;					
	    data_->push_back('b');					
	    write_string(l_bytes.substr(l_pos, l_sublen));	
	    l_bytesLen -= l_sublen;					
	    l_pos += l_sublen;						
	}								
	data_->append(1, 'B');						
	write_string(l_bytes.substr(l_pos, l_bytesLen));

    }									
    
  void bbdf_encoder::write_boolean(boost::shared_ptr<Boolean> object) { 
	if (object == NULL) {
	    write_null();
	    return;
	}
	data_->append(1, object->value() ? 'T' : 'F');

    }

  void bbdf_encoder::write_date(boost::shared_ptr<Date> object) {
    if (object == NULL) {
      write_null();
      return;
    }
    data_->append(1, 'd');
    write_long64(object->value());
  }

  void bbdf_encoder::write_double(boost::shared_ptr<Double> object) {
    if (object == NULL) {
      write_null();
      return;
    }
    data_->append(1, 'D');
    long long dvalue = object->value();
    // tranfer 64-bit double to a 64-bit long
    long long *value = (long long*) & dvalue;
    write_long64(*value);
  }
  
  void bbdf_encoder::write_integer(boost::shared_ptr<Integer> object) {
    if (object == NULL) {
      write_null();
      return;
    }
    data_->append(1, 'I');
    int value = object->value();
    
    int b32 = value >> 24;
    int b24 = (value >> 16) & 0x000000FF;
    int b16 = (value >> 8) & 0x000000FF;
    int b8 = value & 0x000000FF;
    
    data_->append(1, (char)b32);
    data_->append(1, (char)b24);
    data_->append(1, (char)b16);
    data_->append(1, (char)b8);
  }

  void bbdf_encoder::write_unsigned_integer(boost::shared_ptr<UnsignedInteger> object) {
    if (object == NULL) {
      write_null();
      return;
    }
    data_->append(1, 'U');
    uint32_t value = object->value();
    
    uint32_t b32 = value >> 24;
    uint32_t b24 = (value >> 16) & 0x000000FF;
    uint32_t b16 = (value >> 8) & 0x000000FF;
    uint32_t b8 = value & 0x000000FF;
    
    data_->append(1, (char)b32);
    data_->append(1, (char)b24);
    data_->append(1, (char)b16);
    data_->append(1, (char)b8);
  }
  
  void bbdf_encoder::write_vector(boost::shared_ptr<Vector> object) {
    if (object == NULL) {
      write_null();
      return;
    }
    
    data_->append(1, 'V');
    vector<boost::shared_ptr<Object> > l = object->value();
  
    for (vector<boost::shared_ptr<Object> >::iterator i = l.begin(); i != l.end(); i++) {
      write_object(*i);
    }
    data_->append(1, 'z');
    
  }
  
  void bbdf_encoder::write_long(boost::shared_ptr<Long> object) {
    if (object == NULL) {
      write_null();
      return;
    }
    data_->append(1, 'L');
    write_long64(object->value());

  }

  void bbdf_encoder::write_map(boost::shared_ptr<Map> object) {
    data_->append(1, 'M');
    Map::basic_type m = object->value();
    for (Map::basic_type::iterator i = m.begin(); i != m.end(); i++) {
      //writeObject(const_cast<String*>(&((*i).first)));
      data_->append(1, 'S');
      write_string((*i).first);
      write_object((*i).second);
    }
    data_->append(1, 'z');
    
  }

  void bbdf_encoder::write_long64(long long value) {
    long long b64 = (value >> 56) & 0x00000000000000FF;
    long long b56 = (value >> 48) & 0x00000000000000FF;
    long long b48 = (value >> 40) & 0x00000000000000FF;
    long long b40 = (value >> 32) & 0x00000000000000FF;
    long long b32 = (value >> 24) & 0x00000000000000FF;
    long long b24 = (value >> 16) & 0x00000000000000FF;
    long long b16 = (value >> 8) & 0x00000000000000FF;
    long long b8 = value & 0x00000000000000FF;
    
    data_->append(1, (char)b64);
    data_->append(1, (char)b56);
    data_->append(1, (char)b48);
    data_->append(1, (char)b40);
    data_->append(1, (char)b32);
    data_->append(1, (char)b24);
    data_->append(1, (char)b16);
    data_->append(1, (char)b8);
	
  }
  
  void bbdf_encoder::write_null() {
    data_->append(1, 'N');
  }
 
  void bbdf_encoder::write_object(boost::shared_ptr<Object> object) {
	if (object->classname() == "Binary") {
	  boost::shared_ptr<Binary> obj = boost::dynamic_pointer_cast<Binary>(object);
	  write_binary(obj);
	    return;
	}
	if (object->classname() == "Boolean") {
	  write_boolean(boost::dynamic_pointer_cast<Boolean>(object));
	    return;
	}
	if (object->classname() == "Date") {
	  write_date(boost::dynamic_pointer_cast<Date>(object));
	    return;
	}
	if (object->classname() == "Double") {
	  write_double(boost::dynamic_pointer_cast<Double>(object));
	    return;
	}
	if (object->classname() == "Integer") {
	  write_integer(boost::dynamic_pointer_cast<Integer>(object));
	    return;
	}
	if (object->classname() == "UnsignedInteger") {
	  write_unsigned_integer(boost::dynamic_pointer_cast<UnsignedInteger>(object));
	    return;
	}
	if (object->classname() == "Vector") {
	  write_vector(boost::dynamic_pointer_cast<Vector>(object));
	    return;
	}
	if (object->classname() == "Long") {
	  write_long(boost::dynamic_pointer_cast<Long>(object));
	    return;
	}
	if (object->classname() == "Map") {
	  write_map(boost::dynamic_pointer_cast<Map>(object));
	    return;
	}
	if (object->classname() == "Null") {
	  write_null();
	  return;
	}
	if (object->classname() == "String") {
	  write_string(boost::dynamic_pointer_cast<String>(object));
	    return;
	}
	if (object->classname() == "Xml") {
	  write_xml(boost::dynamic_pointer_cast<Xml>(object));
	    return;
	}
	cerr << "write_object: object is of unknown type " << object->classname() << endl;
	// throw exception, should not get here, really
	throw io_exception(string("bbdf_encoder::write_object(): come on, really, what kind of Object is ").append(object->classname()).append("!"));
    }

  void bbdf_encoder::write_string(boost::shared_ptr<String> object) {
	data_->append(1, 'S');
	write_string(object->value());
    }
    
  void bbdf_encoder::write_xml(boost::shared_ptr<Xml> object) {
	data_->append(1, 'X');
	write_string(object->value());
    }
  
    void bbdf_encoder::write_string(const string& value) {
      uint32_t length = (uint32_t)value.length();

      uint32_t b32 = length >> 24;
      uint32_t b24 = (length >> 16) & 0x000000FF;
      uint32_t b16 = (length >> 8) & 0x000000FF;
      uint32_t b8 = length & 0x000000FF;

      data_->append(1, (char)b32);
      data_->append(1, (char)b24);
      data_->append(1, (char)b16);
      data_->append(1, (char)b8);
      
      //unsigned short slen = (unsigned short) value.length();
      //	unsigned char b16 = slen >> 8;
      //	unsigned char b8 = slen & 0x00FF;

      //data_->append(1, b16);
      //	data_->append(1, b8);
      
      data_->append(value.data(), (size_t)length);
    }
}
