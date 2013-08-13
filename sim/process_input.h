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
	bool m_debug;			// �Ƿ���debug��ʽ��true��
	int m_id;				// �ĵ���id��index
	int m_lat;				// �ĵ���γ�ȵ�index
	int m_lng;				// �ĵ��о��ȵ�index
	int m_name;				// �ĵ������Ƶ�index
	int m_addr;				// �ĵ��е�ַ��index
	int m_split_addr;		// �ĵ��зָ��ַ��index
	int m_addr_level;		// �ĵ��е�ַ�����index
	int m_type;				// �ĵ����������index
	int m_type_name;		// �ĵ����������Ƶ�index
	int m_source;			// �ĵ�����Դ��index
	int m_province;			// �ĵ���ʡ��index
	int m_city;				// �ĵ����е�index
	int m_district;			// �ĵ�������index
	int m_accurate_type;	// �ĵ����ӵ�ʶ������index
	int m_tel;				// �ĵ��е绰��index
	int m_status;			// �ĵ�����Ч״̬��index
	int m_max_index;		// �ĵ�����Ҫʹ���ֶε�����index����Ҫ�����ڼ��ǰ��������Ƿ�������
	int m_thread_num;		// ʹ���߳�����
    
	std::string m_delim;			// �ָ���
    
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
