/*
 * logger.h
 *
 *  Created on: 2012-8-21
 *      Author: lzz
 */

#ifndef LOGGER_H_
#define LOGGER_H_
#include "kthttp.h"

using namespace std;
using namespace kyototycoon;

// logger implementation
class RichLogger : public HTTPServer::Logger {
    
 public:
  // constructor
  explicit RichLogger() : strm_(NULL), lock_() {};
  // destructor
  ~RichLogger() {
    if (strm_) close();
  };

  bool open_(){
      struct tm *ptr;
      time_t now;
      now = time(NULL);
      ptr = localtime(&now);
      char strNow[80];
      strftime(strNow,sizeof(strNow),".%y%m%d",ptr);
      string logName(log_name_);
      logName.append(strNow);
      const char* path = logName.c_str();
      if (strm_) return false;
      if (path && *path != '\0' && std::strcmp(path, "-")) {
        std::ofstream* strm = new std::ofstream;
        strm->open(path, std::ios_base::out | std::ios_base::binary | std::ios_base::app);
        if (!*strm) {
          delete strm;
          return false;
        }
        strm_ = strm;
      } else {
        strm_ = &std::cout;
      }
      return true;
  }
  // open the stream
  bool open(const char* path) {
    log_name_.assign(path);
    return true;
  }
  // close the stream
  void close() {
    if (!strm_) return;
    if (strm_ != &std::cout) delete strm_;
    strm_ = NULL;
  }
  // process a log message.
  void log(Kind kind, const char* message) {
    lock_.lock();
    open_();
    if (!strm_) {
        lock_.unlock();
        return;
    }
    char date[48];
    datestrwww(kc::nan(), kc::INT32MAX, 6, date);
    const char* kstr = "MISC";
    switch (kind) {
      case ThreadedServer::Logger::DEBUG: kstr = "DEBUG"; break;
      case ThreadedServer::Logger::INFO: kstr = "INFO"; break;
      case ThreadedServer::Logger::SYSTEM: kstr = "SYSTEM"; break;
      case ThreadedServer::Logger::ERROR: kstr = "ERROR"; break;
    }

    *strm_ << date << ": [" << kstr << "]: " << message << "\n";
    strm_->flush();
    close();
    lock_.unlock();
  }
 private:
  string log_name_;
  std::ostream* strm_;
  kc::Mutex lock_;
};

int logger_init(string& file_name,uint32_t level);
void logger_release();
RichLogger* logger_get(int type = 0);
uint32_t logger_level();
void logger_log(RichLogger::Kind kind, const char* format, ...);
#endif /* LOGGER_H_ */
