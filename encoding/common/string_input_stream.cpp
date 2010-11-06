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
#include "encoding/common/string_input_stream.h"

using namespace BBDF;
using namespace BBDF::exceptions;

namespace BBDF {

  string_input_stream::string_input_stream(const unsigned char* str, int size): pointer_(0), length_(size) {
    unsigned char* newStr = (unsigned char*) malloc(size);
    memcpy(newStr, str, size);
    str_ = newStr;
  }
  
  string_input_stream::~string_input_stream() throw() {
    free((void*)str_);
  }
  
  bool string_input_stream::eof() throw(io_exception) {
    return pointer_ >= length_;
  }
  
  int string_input_stream::read() throw(io_exception) {
    if (eof()) {
      throw io_exception("string_input_stream::read(): read past end of stream");
    }
    
    unsigned char c;
    c = str_[pointer_];
    pointer_++;
    return (int)c;
  }
}

