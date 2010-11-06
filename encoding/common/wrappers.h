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
#ifndef BBDF_WRAPPERS_H
#define BBDF_WRAPPERS_H

#include "boost/shared_ptr.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <hash_map>

using namespace std;

  struct bytes_t {
    const char* bytes;
    size_t size;
  };



namespace BBDF {

    namespace wrappers {
	
	///Non-copyable base class for wrapper object classes.
	class Object {
	public:
	Object() : _classname("Object") {
		_created++;
	    }
	    virtual ~Object() {
		_deleted++;
	    }
	    string classname() const {
		return _classname;
	    }
	    static int created() {
		return _created;
	    }
	    static int deleted() {
	      return _deleted;
	    }
	    static int copied() {
	      return _copied;
	    }
	
	Object(const Object& other) : _classname(other._classname) {
		_copied++;
	    }
	
	    Object operator=(const Object& right) {
		_copied++;
		return * this;
	    }
	protected:
	    string _classname;
	private:
	    static int _created;
	    static int _deleted;
	    static int _copied;
	};

	/// null wrapper class
	class Null : public Object {
	public:
	    typedef void* basic_type;
	    Null() {
		_classname = string("Null");
	    }
	    virtual basic_type value() {
		return NULL;
	    }
	};

	/// ref wrapper class
	class Ref : public Null {
	public:
	    Ref(int value) {
		_classname = string("Ref");
	    }
	};

	/// remote wrapper class
	class Remote : public Null {
	public:
	    Remote() {
		_classname = string("Remote");
	    }
	};


	/// long long (64) wrapper class
	class Long : public Object {
	public:
	    typedef long long basic_type;
	Long(basic_type value): _value(value) {
		_classname = string("Long");
	    }
	    basic_type value() {
		return _value;
	    }
	    operator basic_type() {
		return _value;
	    }
	protected:
	    basic_type _value;
	};


	/// int (32) wrapper class
	class Integer : public Object {
	public:
	    typedef int32_t basic_type;
	Integer(int32_t value): _value(value) {
		_classname = string("Integer");
	    }
	    int32_t value() {
	      return _value;
	    }
	    operator int() {
	      return _value;
	    }
	protected:
	    basic_type _value;
	};
	
	/// uint (32) wrapper class
	class UnsignedInteger : public Object {
	public:
	    typedef uint32_t basic_type;
	UnsignedInteger(uint32_t value): _value(value) {
		_classname = string("UnsignedInteger");
	    }
	    uint32_t value() {
	      return _value;
	    }
	    operator uint() {
	      return _value;
	    }
	protected:
	    basic_type _value;
	};
	

	/// bool wrapper class
	class Boolean : public Object {
	public:
	    typedef bool basic_type;
	Boolean(bool value): _value(value) {
		_classname = string("Boolean");
	    }
	    bool value() {
		return _value;
	    }
	    operator bool() {
		return _value;
	    }
	protected:
	    basic_type _value;
	};


	/// date wrapper class, same as Long
	class Date : public Long {
	public:
	Date(Long::basic_type value) : Long(value) {
		_classname = string("Date");
	    }
	};


	/// binary wrapper class
	class Binary : public Object {
	public:
	    typedef string basic_type;
	Binary(string value) : _value(value) {
		_classname = string("Binary");
	    }
	    string value() {
		return _value;
	    }
	    operator const char*() {
		return _value.c_str();
	    }
	protected:
	    basic_type _value;
	};


	/// double wrapper class
	class Double : public Object {
	public:
	    typedef double basic_type;
	Double(double value) : _value(value) {
		_classname = string("Double");
	    }
	    double value() {
		return _value;
	    }
	    operator double() {
		return _value;
	    }
	protected:
	    basic_type _value;
	};


	/// std::vector wrapper class.
	/**
	 * Usage: create a std::vector<Object*> (basic_type) to
	 * which add _only_ pointers to objects created with new, i.e
	 * <pre>
	 * 	// regular STL vector
	 * 	std::vector<Object*> lst;
	 * 	// data to put in, notice "new"
	 * 	Integer* integer = new Integer(10);
	 * 	String* str = new String("foo bar");
	 * 	lst.push_back(integer);
	 * 	lst.push_back(str);
	 * 	// wrap vector
	 * 	Vector ovector(lst);
	 * 	// use it as parameter to method call
	 * </pre>
	 * When the Vector goes out of scope, the dtor iterates the wrapped
	 * std::vector<Object*> and deletes all Objects.
	 */
	class Vector : public Object {
	public:
	  typedef std::vector<boost::shared_ptr<Object> > basic_type;

	Vector(const basic_type& value) : 
	  _value(value) {
	    _classname = string("Vector");
	  }
	  /// Deletes all Object in the embedded std::vector<Object*>
	  virtual ~Vector() {
	    //for (basic_type::iterator i = _value.begin(); i != _value.end(); i++) {
	    //  Object* obj = *i;
	    //  delete obj;
	  }
	  
	  basic_type value() {
	    return _value;
	  }
	  operator basic_type() {
	    return _value;
	  }
	protected:
	  basic_type _value;
	private:
	  Vector(const Vector& other) {
	  }
	  Vector operator=(const Vector& other) {
		return *this;
	  }
	};
	
	/// std::string wrapper class
	class String : public Object {
	public:
	    typedef std::string basic_type;
	String(const string& value) : _value(value) {
		_classname = string("String");
	    }
	    string value() const {
		return _value;
	    }
	    operator basic_type() {
		return _value;
	    }
	    bool operator<(const String& other) const {
		return _value < other._value;
	    }
	protected:
	    string _value;
	};

	/// std::map wrapper class
	class Map : public Object {
	public:
      typedef hash_map<std::string, boost::shared_ptr<Object> > basic_type;
	Map(const basic_type& value) : _value(value) {
	    _classname = string("Map");
      }
      virtual ~Map() {
	    //for (basic_type::iterator i = _value.begin(); i != _value.end(); i++) {
	    //	      std::pair<string,Object*> key_val = *i;
	    //	      delete key_val.second;
      }
	
	  basic_type value() {
		return _value;
	  }
	  operator basic_type() {
	    return _value;
	  }
	protected:
	    basic_type _value;///< Map's internal std::map
	private:
	    Map(const Map& other) {
	    }
	    Map operator=(const Map& other) {
	      return *this;
	    }
	};
	
	/// xml wrapper class
	class Xml : public String {
	public:
	    typedef std::string basic_type;
	Xml(const string& value) : String(value) {
		_classname = string("Xml");
	    }
	};

	/// fault wrapper class
	class Fault : public Object {
	public:
	    typedef std::string basic_type;
	Fault(const string& code, const string& message):
	    _code(code), _message(message) {
		_classname = string("Fault");
	    }
	Fault(const Fault& other) :
	    _code(other._code), _message(other._message) {
		;
	    }
	    string value() {
		return string("HessianFault: {code=").append(_code).append(", message=").append(_message).append("}");
	    }
	    string code() const {
		return _code;
	    }
	    string message() const {
		return _message;
	    }
	protected:
	    string _code;
	    string _message;
	};
    }
}



#endif 
/*
 *binary						A binary value
 *boolean					The byte 'F' represents false and the byte 'T' represents true
 *date							Date represented by a 64-bits long of milliseconds since the epoch
 *double						A 64-bit IEEE floating pointer number
A *int							A 32-bit signed integer
 *vector							An ordered vector, like an array
 *long							A 64-bit signed integer
 *map							Represents serialized objects and Maps
 *null							Null represents a null pointer
 *ref							An integer referring to a previous vector or map instance
 *remote						A reference to a remote object
 *string						A 16-bit unicode character string encoded in UTF-8
 *xml							An XML document encoded as a 16-bit unicode character string encoded in UTF-8
 */
