#include "core/Datagram.h"
#include <string>

using namespace std;

namespace ServiceCore {
  
  bool Datagram::findValue(const std::string& key, std::string& value)
  {
    bool rval = msg_->get_string(key, value);
    return rval;
  }
  
  bool Datagram::findValue(const std::string& key, int32_t& value)
  {
    bool rval = msg_->get_int32_t(key, value);
    return rval;
  }
  
  bool Datagram::findValue(const std::string& key, uint32_t& value)
  {
    bool rval = msg_->get_uint32_t(key, value);
    return rval;
  }
  
  bool Datagram::findValue(const std::string& key, time_t& value)
  {
    bool rval = msg_->get_date(key, value);
    return rval;
  }
  
  bool Datagram::findValue(const std::string& key, bool& value)
  {
    bool rval = msg_->get_boolean(key, value);
    return rval;
  }
  
  bool Datagram::findValue(const std::string& key, boost::shared_ptr<Datagram>& value)
  {
    boost::shared_ptr<HessianMsg> msg;
    bool rval = msg_->get_msg(key, msg);
    
    if ( rval && msg ) {
      boost::shared_ptr<Datagram> datagram(new Datagram(msg));
      value = datagram;
    }
    
    return rval;
  }
  
  bool Datagram::findBoolean(const std::string& key, bool value) {
    bool v;
    bool success = msg_->get_boolean(key, v);

    if (success)
      return v;

    return value;

  }
  
  bool Datagram::insertKeyAndNull(const std::string& key)
  {
    return msg_->insert_null(key);
  }
  
  bool Datagram::insertKeyAndValue(const std::string& key, int32_t value)
  {
    return msg_->insert_int32_t(key, value);
  }
  
  bool Datagram::insertKeyAndValue(const std::string& key, uint32_t value)
  {
    return msg_->insert_uint32_t(key, value);
  }
  
  bool Datagram::insertKeyAndValue(const std::string& key, bool value)
  {
    return msg_->insert_boolean(key, value);
  }

  bool Datagram::insertKeyAndValue(const std::string& key, time_t value)
  {
    return msg_->insert_date(key, value);
  }
  
  bool Datagram::insertKeyAndValue(const std::string& key, std::string value)
  {
    return msg_->insert_string(key, value);
  }

  bool Datagram::insertKeyAndValue(const std::string& key, const char* value)
  {
    return msg_->insert_string(key, value);
  }
  
  bool Datagram::insertKeyAndValue(const std::string& key, boost::shared_ptr<Datagram> value)
  {
    boost::shared_ptr<HessianMsg> msg = (*value).data();
    boost::shared_ptr<MapType> map(new MapType(*(*msg).values()));
    return msg_->insert_map(key, *map);
  }

  bool Datagram::insertKeyAndVector(const std::string& key, std::vector<boost::shared_ptr<Datagram> > value) {
    std::vector<boost::shared_ptr<wrappers::Object> > v;
    
    std::vector<boost::shared_ptr<Datagram> >::const_iterator i;
    
    for (i = value.begin();
	 i != value.end();
	 ++i) {
      
      boost::shared_ptr<HessianMsg> msg = (**i).data();
      
      boost::shared_ptr<wrappers::Object> obj(new wrappers::Map(*((*msg).values())));
      
      if ( !obj )
	return false;
      
      v.push_back(obj);
    }
    
    return msg_->insert_vector(key, v);
    
  }


  bool Datagram::findVector(const std::string& key, std::vector<boost::shared_ptr<Datagram> >& value) {    
      std::vector<boost::shared_ptr<wrappers::Object> > objVector;                                     

    bool rval = msg_->get_vector(key, objVector);                       
    
    if ( !rval ) {                                                      
      return false;                                                     
    }                                                                   
    
    std::vector<boost::shared_ptr<Datagram> > v;                                         
    
    for ( std::vector<boost::shared_ptr<wrappers::Object> >::iterator i = objVector.begin();       
          i != objVector.end();                                         
          i++ ) {
      boost::shared_ptr<wrappers::Object> obj = (*i);                                               
      
      if ( obj->classname() != "Map" ) {                            
        return false;                                    
      }                                                  
      
      boost::shared_ptr<wrappers::Map> objValue = boost::dynamic_pointer_cast< wrappers::Map >(obj);
      
      if ( !objValue ) {
	return false;
      }
      
      boost::shared_ptr<MapType > map (new MapType((*objValue).value()));
      boost::shared_ptr<HessianMsg> msg(new HessianMsg(map));
      boost::shared_ptr<Datagram> datagram(new Datagram ( msg ));
      v.push_back(datagram);
    }
    
    value = v;                                                          
    return true;                                                        
  }

  bool Datagram::insertKeyAndMap(const std::string& key, std::hash_map<std::string, uint32_t>& value) {
    std::hash_map<std::string, int32_t> newValue;
    
    std::hash_map<std::string, uint32_t>::const_iterator iter;
    
    for ( iter = value.begin();
	  iter != value.end();
	  ++iter )
      {
	newValue[(*iter).first] = (int32_t)(*iter).second;
      }

    return insertKeyAndMap(key, newValue);
  }

  bool Datagram::findMap(const std::string& key, std::hash_map<std::string, boost::shared_ptr<Datagram> >& value) {
    
    boost::shared_ptr<MapType> map(new MapType);
    msg_->get_map(key, *map);

    for ( MapTypeIterator i = (*map).begin();
	  i != (*map).end();
	  ++i )
      {
	boost::shared_ptr<MapType> newMap(new MapType);

	std::string k = (*i).first;
	boost::shared_ptr<wrappers::Object> obj = (*i).second;

	if ( obj->classname() != "Map" ) {
	  return false;
	}
	
	boost::shared_ptr<wrappers::Map> record = boost::dynamic_pointer_cast<wrappers::Map>(obj);	
	*newMap = record->value();
      
	boost::shared_ptr<HessianMsg> msg(new HessianMsg(newMap));
	DatagramPtr datagram(new Datagram(msg));
	
	value[k] = datagram;
      }

    return true;

  }

  bool Datagram::findMap(const std::string& key, std::hash_map<std::string, std::vector<std::string> >& value) 
  {
    
    boost::shared_ptr<MapType> map(new MapType);
    msg_->get_map(key, *map);
    
    for ( MapTypeIterator i = (*map).begin();
	  i != (*map).end();
	  ++i )
      {
	std::string k = (*i).first;
	boost::shared_ptr<wrappers::Object> obj = (*i).second;
	
	if ( obj->classname() != "Vector" ) {
	  return false;
	}

	boost::shared_ptr<wrappers::Vector> objVector = boost::dynamic_pointer_cast< wrappers::Vector >((*i).second);
	std::vector<boost::shared_ptr<Object> > newObjVector = objVector->value();

	std::vector<std::string> v;					     
	
	for ( std::vector<boost::shared_ptr<Object> >::iterator j = newObjVector.begin(); 
	      j != newObjVector.end();
	      ++j ) {			
	  boost::shared_ptr<Object> objString = *j;
	  
	  if ( objString->classname() != "String" ) {                             
	    return false;                                                   
	  }                                                                 
	  
	  boost::shared_ptr<BBDF::wrappers::String> objValue = boost::dynamic_pointer_cast< BBDF::wrappers::String >(objString);
	  v.push_back(objValue->value());				
	}
	
	value[k] =  v;
      }
    
    return true;                                              

  }

  bool Datagram::findMap(const std::string& key, std::hash_map<std::string, std::hash_map<std::string, std::string> >& value)
  {
    
    boost::shared_ptr<MapType> map(new MapType);
    msg_->get_map(key, *map);
    
    for ( MapTypeIterator i = (*map).begin();
	  i != (*map).end();
	  ++i )
      {
	std::string k = (*i).first;
	boost::shared_ptr<wrappers::Object> obj = (*i).second;
	
	if ( obj->classname() != "Map" ) {
	  return false;
	}

	boost::shared_ptr<wrappers::Map> objMap = boost::dynamic_pointer_cast< wrappers::Map >((*i).second);
	std::hash_map<std::string, boost::shared_ptr<Object> > newObjMap = objMap->value();

	std::hash_map<std::string, std::string> m;					     
	
	for ( std::hash_map<std::string, boost::shared_ptr<Object> >::iterator j = newObjMap.begin(); 
	      j != newObjMap.end();
	      ++j ) {		
	  
	  std::string objKey = (*j).first;
	  boost::shared_ptr<Object> objString = (*j).second;
	  
	  if ( objString->classname() != "String" ) {                             
	    return false;                                                   
	  }                                                                 
	  
	  boost::shared_ptr<BBDF::wrappers::String> objValue = boost::dynamic_pointer_cast< BBDF::wrappers::String >(objString);
	  m[objKey] = objValue->value();				
	}
	
	value[k] = m;
      }
    
    return true;                                              

  }

  bool Datagram::insertKeyAndMap(const std::string& key, const std::hash_map<std::string, std::vector<std::string> >& value)
  {
    std::hash_map<std::string, boost::shared_ptr<wrappers::Object> > objMap;

    std::hash_map<std::string, std::vector<std::string> >::const_iterator i;

    for (i = value.begin(); i != value.end(); ++i) {

      std::string k = (*i).first;
      std::vector<std::string> v = (*i).second;

      std::vector<boost::shared_ptr<Object> > objVector;
      
      std::vector<std::string>::const_iterator j;

      for (j = v.begin(); j != v.end(); ++j) {
	boost::shared_ptr<BBDF::wrappers::String> newString(new BBDF::wrappers::String(*j));
	objVector.push_back(newString);
      }
      
      boost::shared_ptr<wrappers::Vector> objVectorPtr(new wrappers::Vector(objVector));
      
      objMap[k] = objVectorPtr;
    }

    return msg_->insert_map(key, objMap);

  }


}; // namespace ServiceCore 

