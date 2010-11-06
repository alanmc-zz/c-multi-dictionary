#ifndef DATAGRAM_H
#define DATAGRAM_H

#include <iostream>
#include <string>
#include <vector>
#include <hash_map>
#include <hash_set>

#include "encoding/common/wrappers.h"
#include "encoding/hessianMsg/HessianMsg.h"

#include "boost/smart_ptr.hpp"

using namespace std;
using namespace BBDF::wrappers;

struct null_deleter
{
  void operator()(void const *) const
  {
  }
};

#define BUILD_INSERT_KEY_AND_VECTOR(valueType, objType)			\
  bool insertKeyAndVector(const std::string& key, const std::vector< valueType >& value) { \
    std::vector<boost::shared_ptr<Object> > objVector;	\
    for ( std::vector< valueType >::const_iterator i = value.begin();		\
	  i != value.end();						\
	  ++i ) {							\
      boost::shared_ptr<objType> obj(new objType ( (*i) ));		\
      objVector.push_back(obj);						\
    }									\
    return msg_->insert_vector(key, objVector);                         \
  }									\
									\
  bool insertKeyAndVector(const char* key, const std::vector< valueType > value) { \
    std::string k(key);							\
    return insertKeyAndVector(k, value);				\
  }								

#define BUILD_INSERT_KEY_AND_SET(valueType)				\
  bool insertKeyAndSet(const std::string& key, const std::hash_set< valueType >& value) { \
    std::vector< valueType > valVector;					\
    for ( std::hash_set< valueType >::const_iterator i = value.begin();	\
	  i != value.end();						\
	  ++i ) {							\
      valVector.push_back(*i);						\
    }									\
    return insertKeyAndVector(key, valVector);				\
  }									\
  									\
  bool insertKeyAndSet(const char* key, const std::hash_set< valueType >& value) { \
    std::string k(key);							\
    return insertKeyAndSet(k, value);					\
  }								

#define BUILD_INSERT_KEY_AND_MAP(valueType, objType)		\
  bool insertKeyAndMap(const std::string& key, const std::hash_map< std::string, valueType >& value) { \
    std::hash_map<std::string, boost::shared_ptr<Object> > objMap; \
    for ( std::hash_map< std::string, valueType >::const_iterator i = value.begin(); \
	  i != value.end();						\
	  ++i ) {							\
      boost::shared_ptr<objType> obj(new objType ( (*i).second ));	\
      objMap[(*i).first] = obj;						\
    }									\
    return msg_->insert_map(key, objMap);				\
  }									\
									\
  bool insertKeyAndMap(const char* key, const std::hash_map< std::string, valueType >& value) { \
    std::string k(key);							\
    return insertKeyAndMap(k, value);					\
  }									



#define BUILD_FIND_VECTOR(valueType, objType)				\
  bool findVector(const std::string& key, std::vector< valueType >& value) {  \
    std::vector<boost::shared_ptr<Object> > objVector;	\
    bool rval = msg_->get_vector(key, objVector);			\
									\
    if ( !rval ) {							\
      return false;							\
    }									\
									\
    std::vector< valueType > v;						\
									\
    for ( std::vector< boost::shared_ptr<Object> >::iterator i = objVector.begin(); \
	  i != objVector.end();						\
	  i++ ) {							\
      boost::shared_ptr<Object> obj = (*i);			\
									\
      if ( (obj->classname() != #objType) &&				\
	   ("BBDF::wrappers::" + obj->classname() != #objType) ) {      \
	return false;							\
      }									\
									\
      boost::shared_ptr<objType> objValue = boost::dynamic_pointer_cast< objType >(obj); \
      v.push_back(objValue->value());					\
    }									\
    value = v;								\
    return true;							\
  }									\
									\
  bool findVector(const char* key,  std::vector< valueType >& value) {  \
    std::string k(key);							\
    return findVector(k, value);					\
  }

#define BUILD_FIND_SET(valueType, objType)				\
  bool findSet(const std::string& key, std::hash_set< valueType >& value) {   \
    std::vector<boost::shared_ptr<Object> > objVector;	\
    bool rval = msg_->get_vector(key, objVector);			\
    									\
    if ( !rval ) {							\
      return false;							\
    }									\
    									\
    std::hash_set< valueType > v;					\
    									\
    for ( std::vector< boost::shared_ptr<Object> >::iterator i = objVector.begin(); \
	  i != objVector.end();						\
	  i++ ) {							\
      boost::shared_ptr<Object> obj = (*i);			        \
      									\
      if ( (obj->classname() != #objType) &&				\
	   ("BBDF::wrappers::" + obj->classname() != #objType) ) {      \
	return false;							\
      }									\
      									\
      boost::shared_ptr<objType> objValue = boost::dynamic_pointer_cast< objType >(obj); \
      v.insert(objValue->value());					\
    }									\
    value = v;								\
    return true;							\
  }									\
									\
  bool findSet(const char* key,  std::hash_set< valueType >& value) {  \
    std::string k(key);							\
    return findSet(k, value);					\
  }


#define BUILD_FIND_MAP(valueType, objType)				\
  bool findMap(const std::string& key, std::hash_map< std::string, valueType >& value) { \
    std::hash_map<std::string, boost::shared_ptr<Object> > objMap; \
    bool rval = msg_->get_map(key, objMap);				\
    									\
    if ( !rval ) {							\
      return false;							\
    }									\
    									\
    std::hash_map< std::string, valueType > m;				\
									\
    for ( std::hash_map< std::string, boost::shared_ptr<Object> >::iterator i = objMap.begin(); \
	  i != objMap.end();						\
	  ++i ) {							\
      boost::shared_ptr<Object> obj = (*i).second;		\
      									\
      if ( (obj->classname() != #objType) &&				\
           ("BBDF::wrappers::" + obj->classname() != #objType) ) {      \
	return false;							\
      }									\
      									\
      boost::shared_ptr<objType> objValue = boost::dynamic_pointer_cast< objType >(obj); \
      m[(*i).first] = objValue->value();				\
    }									\
    value = m;								\
    return true;							\
  }									\
									\
  bool findMap(const char* key,  std::hash_map< std::string, valueType >& value) { \
    std::string k(key);							\
    return findMap(k, value);						\
  }


namespace ServiceCore {
  class Datagram {
    
  public:
  Datagram() :
    msg_ (new HessianMsg) {};
    
  Datagram(boost::shared_ptr<BBDF::HessianMsg> msg) :
    msg_ (msg) {};
    
    ~Datagram() { };
    
    bool findValue(const std::string& key, int32_t& value);    
    bool findValue(const std::string& key, uint32_t& value);    
    bool findValue(const std::string& key, std::string& value);    
    bool findValue(const std::string& key, time_t& value);
    bool findValue(const std::string& key, bool& value);
    bool findValue(const std::string& key, boost::shared_ptr<Datagram>& value);
    //    bool findValue(std::string& key, Datagram*& value);

    bool findValue(const char* key, int32_t& value) {
      std::string k(key);
      return findValue(k, value);
    }
    
    bool findValue(const char* key, uint32_t& value) {
      std::string k(key);
      return findValue(k, value);
    }
    
    bool findValue(const char* key, std::string& value) {
      std::string k(key);
      return findValue(k, value);
    }

    bool findValue(const char* key, time_t& value) {
      std::string k(key);
      return findValue(k, value);
    }

    bool findValue(const char* key, bool& value) {
      std::string k(key);
      return findValue(k, value);
    }

    bool findValue(const char* key, boost::shared_ptr<Datagram>& value) {
      std::string k(key);
      return findValue(k, value);
    }
    
    bool findBoolean(const std::string& key, bool value = false);

    bool findBoolean(const char* key, bool value = false) {
      std::string k(key);
      return findBoolean(k, value);
    }

    bool insertKeyAndNull(const std::string& key);

    bool insertKeyAndValue(const std::string& key, int32_t value);
    bool insertKeyAndValue(const std::string& key, uint32_t value);
    bool insertKeyAndValue(const std::string& key, std::string value);
    bool insertKeyAndValue(const std::string& key, bool value);
    bool insertKeyAndValue(const std::string& key, time_t date);
    bool insertKeyAndValue(const std::string& key, const char*);
    bool insertKeyAndValue(const std::string&, MapType& value);
    //    bool insertKeyAndValue(std::string& key, HessianMsg*);
    bool insertKeyAndValue(const std::string& key, boost::shared_ptr<Datagram>);
    //    bool insertKeyAndValue(std::string& key, Datagram*);

    bool insertKeyAndNull(const char* key) {
      std::string k(key);
      return insertKeyAndNull(k);
    }
    
    bool insertKeyAndValue(const char* key, int32_t value) {
      std::string k(key);
      return insertKeyAndValue(k,value);	
    }

    bool insertKeyAndValue(const char* key, uint32_t value) {
      std::string k(key);
      return insertKeyAndValue(k,value);	
    }

    bool insertKeyAndValue(const char* key, std::string value) {
      std::string k(key);
      return insertKeyAndValue(k,value);	
    }
    
    bool insertKeyAndValue(const char* key, bool value){
      std::string k(key);
      return insertKeyAndValue(k,value);	
    }

    bool insertKeyAndValue(const char* key, time_t value){
      std::string k(key);
      return insertKeyAndValue(k,value);	
    }

    bool insertKeyAndValue(const char* key, const char* value) {
      std::string k(key);
      return insertKeyAndValue(k,value);	
    }
    
    bool insertKeyAndValue(const char* key, MapType& value) {
      std::string k(key);
      return insertKeyAndValue(k,value);	
    }
    
    bool insertKeyAndValue(const char* key, HessianMsg* value) {
      std::string k(key);
      return insertKeyAndValue(k,value);	
    }
    
    bool insertKeyAndValue(const char* key, boost::shared_ptr<Datagram> value){
      std::string k(key);
      return insertKeyAndValue(k,value);	
    }

    //    bool insertKeyAndValue(const char* key, Datagram* value){
    //      std::string k(key);
    //      return insertKeyAndValue(k,value);	
    //    }

    bool insertKeyAndVector(const std::string& key, std::vector<boost::shared_ptr<Datagram> >);

    bool insertKeyAndVector(const char* key, std::vector<boost::shared_ptr<Datagram> >  value) {
      std::string k(key);
      return insertKeyAndVector(k, value);
    }
    
    bool findVector(const std::string& key, std::vector<boost::shared_ptr<Datagram> >&);

    bool findVector(const char* key, std::vector<boost::shared_ptr<Datagram> >& value) {
      std::string k(key);
      return findVector(k, value);
    }
    
    boost::shared_ptr<HessianMsg> data(void) { return msg_; };
    
    BUILD_INSERT_KEY_AND_VECTOR(std::string, BBDF::wrappers::String);
    BUILD_INSERT_KEY_AND_VECTOR(int32_t, Integer);
    BUILD_INSERT_KEY_AND_VECTOR(uint32_t, UnsignedInteger);
    BUILD_INSERT_KEY_AND_VECTOR(time_t, Date);
    BUILD_INSERT_KEY_AND_VECTOR(bool, Boolean);
    
    BUILD_INSERT_KEY_AND_MAP(std::string, BBDF::wrappers::String);
    BUILD_INSERT_KEY_AND_MAP(int32_t, Integer);
    //BUILD_INSERT_KEY_AND_MAP(uint32_t, UnsignedInteger);
    BUILD_INSERT_KEY_AND_MAP(time_t, Date);
    BUILD_INSERT_KEY_AND_MAP(bool, Boolean);
    
    BUILD_FIND_VECTOR(std::string, BBDF::wrappers::String);
    BUILD_FIND_VECTOR(int32_t, Integer);
    BUILD_FIND_VECTOR(uint32_t, UnsignedInteger);
    BUILD_FIND_VECTOR(time_t, Date);
    BUILD_FIND_VECTOR(bool, Boolean);
    
    BUILD_FIND_SET(std::string, BBDF::wrappers::String);
    BUILD_FIND_SET(int32_t, Integer);
    BUILD_FIND_SET(uint32_t, UnsignedInteger);
    BUILD_FIND_SET(time_t, Date);

    BUILD_INSERT_KEY_AND_SET(std::string);
    BUILD_INSERT_KEY_AND_SET(int32_t);
    BUILD_INSERT_KEY_AND_SET(uint32_t);
    BUILD_INSERT_KEY_AND_SET(time_t);

    BUILD_FIND_MAP(std::string, BBDF::wrappers::String);
    BUILD_FIND_MAP(int32_t, Integer);
    BUILD_FIND_MAP(uint32_t, UnsignedInteger);
    BUILD_FIND_MAP(time_t, Date);
    BUILD_FIND_MAP(bool, Boolean);

    bool insertKeyAndMap(const std::string& key, std::hash_map<std::string, uint32_t>& value);

    bool insertKeyAndMap(const char* key, std::hash_map<std::string, uint32_t>& value) {
      std::string k(key);
      return insertKeyAndMap(k, value);
    }
    
    bool findMap(const std::string& key, std::hash_map<std::string, boost::shared_ptr<Datagram> >& value);
    
    bool findMap(const char* key, std::hash_map<std::string, boost::shared_ptr<Datagram> >& value) {
      std::string k(key);
      return findMap( k, value );
    }

    bool insertKeyAndMap(const std::string& key, const std::hash_map<std::string, std::vector<std::string> >& value);
    
    bool insertKeyAndMap(const char* key, const std::hash_map<std::string, std::vector<std::string> >& value) {
      std::string k(key);
      return insertKeyAndMap( k, value );
    }

    bool findMap(const std::string& key, std::hash_map<std::string, std::vector<std::string> >& value);
    
    bool findMap(const char* key, std::hash_map<std::string, std::vector<std::string> >& value) {
      std::string k(key);
      return findMap( k, value );
    }

    bool findMap(const std::string& key, std::hash_map<std::string, std::hash_map<std::string, std::string> >& value);
    
    bool findMap(const char* key, std::hash_map<std::string, std::hash_map<std::string, std::string> >& value) {
      std::string k(key);
      return findMap( k, value );
    }

    
    void decode (Bytes bytes, std::size_t len) {
      msg_->decode(bytes, len);
    }
    
    bytes_t encode() {
      return msg_->encode();
    }

    MapTypeIterator begin() {
      return (*msg_).begin();
    }
    
    MapTypeIterator end() {
      return (*msg_).end();
    }
    

  private:
    boost::shared_ptr<HessianMsg> msg_;
  };

  typedef boost::shared_ptr<ServiceCore::Datagram> DatagramPtr;


  
}; // class Datagram

#endif // ServiceCore

