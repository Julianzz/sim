/*
 * logger.cpp
 *
 *  Created on: 2012-8-21
 *      Author: lzz
 */

#include "logger.h"
     
static RichLogger* rich_logger = NULL;
static uint32_t rich_logger_level = ThreadedServer::Logger::SYSTEM | ThreadedServer::Logger::ERROR;

int logger_init(string& file_name,
        uint32_t level = ThreadedServer::Logger::SYSTEM|ThreadedServer::Logger::ERROR) {
            
    if (rich_logger==NULL){
        rich_logger = new RichLogger();
        rich_logger->open(file_name.c_str());
        rich_logger_level = level;
    }
    return 0;
}

void logger_release(){
    if(rich_logger){
        rich_logger->close();
        delete rich_logger;
        rich_logger = NULL;
    }
}

RichLogger* logger_get(int type){
    return rich_logger;
}

uint32_t logger_level(){
    return rich_logger_level;
}

/**
 * Log a message.
 */

void logger_log(RichLogger::Kind kind, const char* format, ...) {
  if (!rich_logger || !(kind & rich_logger_level)) return;
  std::string msg;
  va_list ap;
  va_start(ap, format);
  kc::vstrprintf(&msg, format, ap);
  va_end(ap);
  rich_logger->log(kind, msg.c_str());
}
void logger_log_v(RichLogger::Kind kind, const char* format, va_list ap) {
  _assert_(format);
  if (!rich_logger || !(kind & rich_logger_level)) return;
  std::string msg;
  kc::vstrprintf(&msg, format, ap);
  rich_logger->log(kind, msg.c_str());
}
