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
#include "encoding/common/zlibdec.h"
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <zlib.h>
#include <vector>

using namespace std;

zlibdec::zlibdec(unsigned int buf_size) : _buf_size(buf_size) {
}

std::string 
zlibdec::compress_string(const std::string& str,
			 int compressionlevel)
{
  z_stream zs;                        // z_stream is zlib's control structure
  memset(&zs, 0, sizeof(zs));

  if (deflateInit(&zs, compressionlevel) != Z_OK)
    throw(std::runtime_error("deflateInit failed while compressing."));

  zs.next_in = (Bytef*)str.data();
  zs.avail_in = str.size();           // set the z_stream's input

  int ret;
  char outbuffer[32768];
  std::string outstring;

  // retrieve the compressed bytes blockwise
  do {
    zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
    zs.avail_out = sizeof(outbuffer);

    ret = deflate(&zs, Z_FINISH);

    if (outstring.size() < zs.total_out) {
      // append the block to the output string
      outstring.append(outbuffer,
		       zs.total_out - outstring.size());
    }
  } while (ret == Z_OK);

  deflateEnd(&zs);

  if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
    std::ostringstream oss;
    oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
    throw(std::runtime_error(oss.str()));
  }

  return outstring;
}

std::string 
zlibdec::decompress_string(const std::string& str)
{
  z_stream zs;                        // z_stream is zlib's control structure
  memset(&zs, 0, sizeof(zs));

  if (inflateInit(&zs) != Z_OK)
    throw(std::runtime_error("inflateInit failed while decompressing."));

  zs.next_in = (Bytef*)str.data();
  zs.avail_in = str.size();

  int ret;
  char outbuffer[32768];
  std::string outstring;

  // get the decompressed bytes blockwise using repeated calls to inflate
  do {
    zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
    zs.avail_out = sizeof(outbuffer);

    ret = inflate(&zs, 0);

    if (outstring.size() < zs.total_out) {
      outstring.append(outbuffer,
		       zs.total_out - outstring.size());
    }

  } while (ret == Z_OK);

  inflateEnd(&zs);

  if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
    std::ostringstream oss;
    oss << "Exception during zlib decompression: (" << ret << ") "
	<< zs.msg;
    throw(std::runtime_error(oss.str()));
  }

  return outstring;
}


string zlibdec::decompress(const char* buffer, unsigned int len) throw (zlib_exception) {
	// result
	string decstr;
	// temp dec buffer
	vector<char> decbuf(_buf_size);
	// setup z-stream
	z_stream str;
	str.zalloc = (alloc_func)0;
	str.zfree = (free_func)0;
	str.opaque = (voidpf)0;

	str.next_in = (Bytef*)buffer;
	str.avail_in = len;

	str.next_out = (Bytef*)(&decbuf[0]);
	str.avail_out = _buf_size;

	// init with gzip headers support
	int err = inflateInit2(&str, 15 + 32); // for gzip decoding
	if (err != Z_OK) {
		throw zlib_exception("zlibdec::decompress(): inflateInit2() failed");
	}

	// inflate loop
	for (;;) {
		err = inflate(&str, Z_SYNC_FLUSH);
		// number of bytes decoded
		int num_bytes_dec = _buf_size - str.avail_out;
		// append to output
		if (num_bytes_dec > 0) {
			decstr.append(&decbuf[0], num_bytes_dec);
		}
		// test for end of stream
		if (err == Z_STREAM_END) {
			break;
		}
		// if stream is OK and the whole buffer was used we have to read again
		if (err == Z_OK && str.avail_out == 0) {
			str.avail_out = _buf_size;
			str.next_out = (Bytef*)(&decbuf[0]);
		}
		else {
			// status not handled
			switch (err) {
			case Z_OK : throw zlib_exception("zlibdec::decompress(): Z_OK");
			case Z_STREAM_END : throw zlib_exception("zlibdec::decompress(): Z_STREAM_END");
			case Z_NEED_DICT : throw zlib_exception("zlibdec::decompress(): Z_NEED_DICT");
			case Z_ERRNO : throw zlib_exception("zlibdec::decompress(): Z_ERRNO");
			case Z_STREAM_ERROR : throw zlib_exception("zlibdec::decompress(): Z_STREAM_ERROR");
			case Z_DATA_ERROR : throw zlib_exception("zlibdec::decompress(): Z_DATA_ERROR");
			case Z_MEM_ERROR : throw zlib_exception("zlibdec::decompress(): Z_MEM_ERROR");
			case Z_BUF_ERROR : throw zlib_exception("zlibdec::decompress(): Z_BUF_ERROR");
			case Z_VERSION_ERROR : throw zlib_exception("zlibdec::decompress(): Z_VERSION_ERROR");
			default : throw zlib_exception("zlibdec::decompress(): unhandled inflate status");
			}
		}
	}

	// finish
	err = inflateEnd(&str);
	if (err != Z_OK) {
		throw zlib_exception("zlibdec::decompress(): inflateEnd() failed");
	}
	// return
	return decstr;
}

string zlibdec::decompress(const string& buffer) throw (zlib_exception) {
	return decompress(buffer.c_str(), buffer.length());
}

