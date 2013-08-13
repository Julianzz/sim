#include "fetch_data.h"
#include "level_io.h"
#include "StringUtility.h"

static CLevelIOPool* pool_ = NULL;
static CLevelIOFactory* factory_ = NULL;
static const int MaxConns = 20;

static vector<string> FetchFields;
static const string tableName("pp_poi");

static string fields_[] = {
    "manual_handle",
    "manual_sign",
    "insert_time",
    "update_time",
    "source",
    "attribute_source",
    "rawid",
    "rawurl",
    "province",
    "city",
    "district",
    "area_code",
    "name",
    "address",
    "phone",
    "longitude",
    "latitude",
    "category",
    "category_code",
    "status",
    "invalid_status",
    "alias",
    "zone",
    "tag",
    "important_level",
    "reliability_level",
    "regional_heat_level",
    "sort_factor_json",
    "split_address",
    "address_level",
    "reserve1",
    "reserve2",
    "reserve3"
};

int FetchService::init(const string& config) {
    factory_ = new CLevelIOFactory(config);
    pool_ = new CLevelIOPool(*factory_);
    pool_->init(MaxConns);
    FetchFields.clear();
    for(size_t i=0; i< sizeof(fields_)/sizeof(fields_[0]); i++ ){
        FetchFields.push_back(fields_[i]);
    }
    return 0;
}

void FetchService::release() {
    if( pool_ != NULL ) { 
        delete pool_;
    }
    if( factory_ != NULL ) {
        delete factory_;
    }
}


int FetchService::fetchPP(const string idStr, vector<string>& results ) {
    IDType id = StringUtility::Str2ULongLong(idStr) ; 
    CLevelIOPool::Connection conn = pool_->get();
    int ret = conn->get(tableName,id,FetchFields,results);
    conn.release();
    return ret;
    
}
int FetchService::fetchPP(const string idStr, map<string,string>& results ) {
    IDType id = StringUtility::Str2ULongLong(idStr) ; 
    CLevelIOPool::Connection conn = pool_->get();
    int ret = conn->get(tableName,id,FetchFields,results);
    conn.release();
    return ret;
}
