#ifndef PROCESS_INPUT_H_EY2MOZED
#define PROCESS_INPUT_H_EY2MOZED

#include "json/json.h"
#include <string>
#include <assert.h>

#include "sim.h"
#include "process_sync.h"

class Record;

class Parser {
public:
    virtual ~Parser() {};
public:
    virtual bool init() = 0;
    virtual bool parse(const std::string&, Record* record ) = 0;
};

class IndexParser: public Parser {
    
private:
	bool m_debug;			// 是否是debug方式，true是
	int m_id;				// 文档中id的index
	int m_lat;				// 文档中纬度的index
	int m_lng;				// 文档中经度的index
	int m_name;				// 文档中名称的index
	int m_addr;				// 文档中地址的index
	int m_split_addr;		// 文档中分割地址的index
	int m_addr_level;		// 文档中地址级别的index
	int m_type;				// 文档中类型码的index
	int m_type_name;		// 文档中类型名称的index
	int m_source;			// 文档中来源的index
	int m_province;			// 文档中省的index
	int m_city;				// 文档中市的index
	int m_district;			// 文档中区的index
	int m_accurate_type;	// 文档中子点识别结果的index
	int m_tel;				// 文档中电话的index
	int m_status;			// 文档中有效状态的index
	int m_max_index;		// 文档中需要使用字段的最大的index，主要是用于检测前面的配置是否有问题
	int m_thread_num;		// 使用线程数量
    
	std::string m_delim;			// 分隔符
    
public:
    virtual bool init();
    virtual bool parse(const std::string& data , Record*record );
    
};

class InputItem {
    
private:
    Record record;
public:
    Record& getRecord() {
        return record;
    }
};

enum InputState {
    INPUT_SUCCESS = 0,
    INPUT_IO_ERROR,
    INPUT_PARSE_ERROR,
    INPUT_UNKNOWN_ERROR
};

class BasicInput {
    
public:
    
    //TODO, parser should input
    BasicInput ():parser_(NULL) {
    }
        
    virtual ~BasicInput () {
        //delete parser_;
        if( ifs_.is_open()) {
            ifs_.close();
        }
    };
    
    virtual bool config(const std::string& fileName,Parser* parser) {
        ifs_.open(fileName.c_str(),std::ifstream::in);
        if (! ifs_.is_open() ) {
            std::cout<<" can not open json :file = " << fileName <<std::endl;
            return false;
        };
        parser_ = parser;
        if( parser == NULL ) {
            std::cout<<"error ,please privde parser ";
            return false;
        }
        return true;
    };
    
    virtual InputState next( Record* record ) {
        
        assert( ifs_.is_open() );
        assert( parser_ != NULL );
        assert ( record != NULL );
        
        std::string line;
		if (!getline(ifs_, line)) {
			return INPUT_IO_ERROR;
		}
        bool ret = parser_->parse(line, record);
        if( !ret ) {
            std::cout <<" parse data error " <<std::endl;
            return INPUT_PARSE_ERROR;
        }
        return INPUT_SUCCESS;
    }
    
    virtual bool isFinish() {
        return ifs_.eof();
    }
    
private:
    Parser* parser_;
    std::ifstream ifs_;
    
};

class MutexInput {
private:
    BasicInput* input_;
    Locker lock_;
    
public:
    MutexInput(BasicInput* input):input_(input) {
    }
    
    virtual ~MutexInput() {
        //error should delet input
        //delete input_;
    }
    
public:
    
    virtual InputState next( Record* record ) {
        lock_.lock();
        InputState ret = input_->next( record );
        lock_.unlock();
        return ret;
    }
    
    virtual bool isFinish() {
        lock_.lock();
        bool ret = input_->isFinish();
        lock_.unlock();
        return ret;    
    }
};


class ResultItem {

};

class BasicOutput {
public:
    BasicOutput();
    virtual ~BasicOutput() {};
};



#endif /* end of include guard: PROCESS_READER_H_EY2MOZED */
