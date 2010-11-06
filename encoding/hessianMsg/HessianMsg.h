#ifndef BBDF_MSG_H
#define BBDF_MSG_H

#include <iostream>
#include <string>
#include <vector>
#include <hash_map>
#include "encoding/common/wrappers.h"
#include "boost/shared_ptr.hpp"

using namespace std;
using namespace BBDF;

namespace BBDF {
  
  typedef std::hash_map<std::string, boost::shared_ptr<wrappers::Object> > MapType;
  typedef std::hash_map<std::string, boost::shared_ptr<wrappers::Object> >::iterator MapTypeIterator;
  typedef std::vector<boost::shared_ptr<wrappers::Object> > VectorType;
  typedef const unsigned char* Bytes;

  class HessianMsg {
    
  public:
  HessianMsg() :
    values_(new MapType) {};
    
  HessianMsg(boost::shared_ptr<MapType> map) :
    values_(map) {};
    
    //~HessianMsg() { 
    // for (MapType::iterator i = (*values_).begin(); 
    //   i != (*values_).end(); 
    //   ++i) 
    //{
    //  Object* obj = (*i).second;
    //  delete obj;
    //  obj = NULL;
    //}
      
      //delete values_;
      //values_ = NULL;
    //}
    
    HessianMsg(const HessianMsg&);
    
    //bool insert_int64_t(const std::string&, int64_t value);
    //bool insert_uint64_t(const std::string&, uint64_t value);

    bool insert_int32_t(const std::string& key, int32_t value);
    bool insert_uint32_t(const std::string&, uint32_t value);
    bool insert_boolean(const std::string& key, bool value);
    bool insert_date(const std::string& key, time_t date);
    bool insert_string(const std::string& key , std::string&);
    bool insert_string(const std::string& key , const char*);
    bool insert_null(const std::string& key);
    bool insert_map(const std::string&, MapType& value);
    bool insert_msg(const std::string& key, boost::shared_ptr<HessianMsg>);
    bool insert_vector(const std::string& key, VectorType&);
    
    boost::shared_ptr<MapType> values(void);

    bool get_int32_t(const std::string& key, int32_t& value);
    bool get_uint32_t(const std::string& key, uint32_t& value);
    bool get_boolean(const std::string& key, bool& value);
    bool get_date(const std::string& key, time_t& value);
    bool get_string(const std::string& key, std::string& value);
    bool get_map(const std::string& key, MapType& value);
    bool get_vector(const std::string& key, VectorType& value);
    bool get_msg(const std::string& key, boost::shared_ptr<HessianMsg>&);

    void decode(Bytes bytes, std::size_t len);
    bytes_t encode(void);

    MapTypeIterator begin() {
      return (*values_).begin();
    }

    MapTypeIterator end() {
      return (*values_).end();
    }
    
  private:
    boost::shared_ptr<MapType> values_;
  };
  
}; // class HessianMsg 

#endif // BBDF_MSG_H

/*
 *binary						A binary value
 *boolean       					The byte 'F' represents false and the byte 'T' represents true
 *date							Date represented by a 64-bits long of milliseconds since the epoch
 *double						A 64-bit IEEE floating pointer number
A *int							A 32-bit signed integer
 *vector						An ordered vector, like an array
 *long							A 64-bit signed integer
 *map							Represents serialized objects and Maps
 *null							Null represents a null pointer
 *ref							An integer referring to a previous vector or map instance
 *remote						A reference to a remote object
 *string						A 16-bit unicode character string encoded in UTF-8
 *xml							An XML document encoded as a 16-bit unicode character string encoded in UTF-8
 */
