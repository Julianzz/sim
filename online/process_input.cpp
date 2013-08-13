#include "process_input.h"

bool IndexParser::init( ) {
    
	this->m_id = 6;
	this->m_name = 12;
	this->m_addr = 13;
	this->m_lat = 15;
	this->m_lng = 16;
	this->m_split_addr = 28;
	this->m_addr_level = 29;
	this->m_type = 18;
	this->m_type_name = 17;
	this->m_source = 4;
	this->m_province = 8;
	this->m_city = 9;
	this->m_district = 10;
	this->m_tel = 14;
	this->m_status = 19;
	this->m_max_index = 29;
    
	this->m_thread_num = 1;
	this->m_delim = "\t";
	this->m_debug = false;
    
    return true;
}

bool IndexParser::parse(const std::string& data , Record*record ) {
	vector<string> vecData;
	split(data, this->m_delim, vecData, "");
    
	record->clear();
	if (vecData.size() < this->m_max_index + 1) {
		record->m_is_null = true;
        std::cout<< "data size is wrong:" << vecData.size() <<","<< this->m_max_index + 1<<std::endl;
		return false; 
	}

	if (this->m_id >= 0) {
		record->m_id = strtoull(vecData[this->m_id].c_str(), NULL, 0);
	}

	if (this->m_lat >= 0) {
		record->m_lat = Str2Double(vecData[this->m_lat]);
	}

	if (this->m_lng >= 0) {
		record->m_lng = Str2Double(vecData[this->m_lng]);
	}

	if (this->m_name >= 0) {
		record->m_name = vecData[this->m_name];
	}

	if (this->m_addr >= 0) {
		record->m_addr = vecData[this->m_addr];
	}

	if (this->m_split_addr >= 0) {
		record->m_split_addr = vecData[this->m_split_addr];
	}

	if (this->m_addr_level >= 0) {
		record->m_addr_level = atoi(
				vecData[this->m_addr_level].c_str());
	}

	if (this->m_type >= 0) {
		record->m_type = vecData[this->m_type];
	}

	if (this->m_type_name >= 0) {
		record->m_type_name = vecData[this->m_type_name];
	}

	if (this->m_source >= 0) {
		record->m_source = vecData[this->m_source];
	}

	if (this->m_province >= 0) {
		record->m_province = vecData[this->m_province];
	}

	if (this->m_city >= 0) {
		record->m_city = vecData[this->m_city];
	}

	if (this->m_district >= 0) {
		record->m_district = vecData[this->m_district];
	}
    /*
	if (this->m_accurate_type >= 0) {
		record->m_accurate_type = vecData[this->m_accurate_type];
	}
    */
	if (this->m_tel >= 0) {
		record->m_tel = vecData[this->m_tel];
	}

	if (this->m_status >= 0) {
		if (vecData[this->m_status] == "0"
				|| vecData[this->m_status] == "1"
				|| vecData[this->m_status] == "2") {
			record->m_status = 0;
		} else if (vecData[this->m_status] == "16") {
			record->m_status = 1;
		} else {
			record->m_status = 0;
		}
	}

	record->m_is_null = false;
    return true;
}