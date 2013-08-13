#ifndef __COMMON_TYPE_H__
#define __COMMON_TYPE_H__
#include <string>
#include <cstdlib>
#include <string.h>

#include "json/json.h"
#include "StringUtility.h"

using namespace std;

typedef unsigned long long IDType;

template <class T> 
struct Converter {
    T convert ( const Json::Value& data);
};

template<>
struct Converter<unsigned long long> {
    unsigned long long convert ( const Json::Value& data) {
        return strtoull(data.asCString(), NULL, 0);
    };
};

template<>
struct Converter<string> {
    string convert ( const Json::Value& data) {
        return data.asString() ;
    }; 
};

template<>
struct Converter<double> {
    double convert ( const Json::Value& data) {
        return StringUtility::Str2Double(data.asString()) ;
    };  
};

template <class T> T json_convert( const string& name, const Json::Value& data ) {
    Converter<T> c;
    if ( data.isMember(name) ) {
        return c.convert(data[name]);
    };  
    return T();
};


#endif
