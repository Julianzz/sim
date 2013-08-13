#include "process_utils.h"
#include <fstream>
#include <assert.h>

bool parseJson(const std::string& fileName, Json::Value& root) {
    std::ifstream ifs_;
    ifs_.open(fileName.c_str(),std::ifstream::in);
    if (!ifs_.is_open() ) {
        std::cout<<" can not open json :file = " << fileName <<std::endl;
        return false;
    };

    Json::Reader reader;
    if (!reader.parse(ifs_, root, false)) {
        std::cout<<"parse file:"<< fileName << " error:"<< reader.getFormatedErrorMessages()<<std::endl;
        ifs_.close();
        return false;
    }
    ifs_.close();
    return true;
}