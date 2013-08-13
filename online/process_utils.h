#ifndef __ONLINE_UTILS_H__
#define __ONLINE_UTILS_H__

#include "json/json.h"

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);   \
  void operator=(const TypeName&)
      
bool parseJson(const std::string& fineName, Json::Value& root) ;

#endif