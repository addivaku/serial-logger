#ifndef SERIAL_LOGGER_SERIAL_LOGGER_HPP_
#define SERIAL_LOGGER_SERIAL_LOGGER_HPP_

#include <HardwareSerial.h>

#include "serial-logger-macros.hpp"

namespace logging {

enum class Level {
  kWarning,
  kInfo,
  kError,
  kDebug,
  kCritical
};

const char * toString(Level level) {
  switch (level) {
    case Level::kWarning:
      return "kWarning";
    case Level::kInfo:
      return "kInfo";
    case Level::kError:
      return "kError";
    case Level::kDebug:
      return "kDebug";
    case Level::kCritical:
      return "kCritical";
    default:
      return "";
  }
}

class Logger {
  private:
  static HardwareSerial * serial_;
  static Level _level;

  public:
  static void setLevel(Level level) {
    _level = level;
  }

  static void setSerial(HardwareSerial* serial) {
    serial_ = serial;
  }

  //! @deprecated: Use variant with different argument order instead
  template<class T>
  static void log(T line, Level level) {
    warning<>(
      "(Deprecated): Please use logging::Logger::log(logger::Level, T).");
    log<>(level, line);
  }

  template <class T>
  static void log(Level level, T data) {
    switch (level) {
      case Level::kDebug:
        debug<>(data);
        break;
      case Level::kInfo:
        info<>(data);
        break;
      case Level::kError:
        error<>(data);
        break;
      case Level::kWarning:
        warning<>(data);
        break;
      case Level::kCritical:
        critical<>(data);
        break;
      default:
        debug(data);
    }
  }

  static void log(Level level, const char * fmt, ...) {
    if (serial_) {
      _LOGGER_STRINGIFY(fmt, fmt);
      if (data) 
        log<>(level, data);
      _LOGGER_STRINGIFY_CLEANUP();
    }
  }

  template<class T>
  static void info(T line){
    auto level = Level::kInfo;
    printId("[INFO    ] ", level);
    printLine<>(line, level);
  };

  static void info(const char * fmt, ...) {
    if (serial_) {
      _LOGGER_STRINGIFY(fmt, fmt);
      if (data)
        info<>(data);
      _LOGGER_STRINGIFY_CLEANUP();
    }
  }

  template<class T>
  static void debug(T line){
    auto level = Level::kDebug;
    printId("[DEBUG   ] ", level);
    printLine<>(line, level);
  };

  static void debug(const char * fmt, ...) {
    if (serial_) {
      _LOGGER_STRINGIFY(fmt, fmt);
      if (data)
        debug<>(data);
      _LOGGER_STRINGIFY_CLEANUP();
    }
  }

  template<class T>
  static void warning(T line){
    auto level = Level::kWarning;
    printId("[WARNING ] ", level);
    printLine<>(line, level);
  };

  static void warning(const char * fmt, ...) {
    if (serial_) {
      _LOGGER_STRINGIFY(fmt, fmt);
      if (data)
        warning<>(data);
      _LOGGER_STRINGIFY_CLEANUP();
    }
  }

  template<class T>
  static void error(T line){
    auto level = Level::kError;
    printId("[ERROR   ] ", level);
    printLine<>(line, level);
  };

  static void error(const char * fmt, ...) {
    if (serial_) {
      _LOGGER_STRINGIFY(fmt, fmt);
      if (data)
        error<>(data);
      _LOGGER_STRINGIFY_CLEANUP();
    }
  }

  template<class T>
  static void critical(T line){
    auto level = Level::kCritical;
    printId("[CRITICAL] ", level);
    printLine<>(line, level);
  };

  static void critical(const char * fmt, ...) {
    if (serial_) {
      _LOGGER_STRINGIFY(fmt, fmt);
      if (data)
        critical<>(data);
      _LOGGER_STRINGIFY_CLEANUP();
    }
  }

  
  static void logLevel(Level level = Level::kInfo) {
    Logger::log(level, "Log level is: %s", toString(level) );
  }


  private:
  template <class T>
  static void printLine(T line, Level level) {
    if (serial_ != nullptr && included_(level) ) {
      serial_->println(line);
    }
  }

  static void printId(const char * id, Level level) {
    if (serial_ != nullptr && included_(level) ) {
      serial_->print(id);
    }
  }

  static bool included_(Level level) noexcept {
    if (_level == Level::kDebug) return true; // debug contains everything
    if (_level == Level::kInfo) {
      // print everything but debug
      return level != Level::kDebug;
    }
    if (_level == Level::kWarning) {
      // print kError, kWarning, kCritical
      return (   level == Level::kWarning 
              || level == Level::kError
              || level == Level::kCritical);
    }
    if (_level == Level::kError) {
      // print kError and kCritical
      return (level == Level::kError || level == Level::kCritical);
    }
    if (_level == Level::kCritical) return level == Level::kCritical;
    return false;
  }
};

HardwareSerial* Logger::serial_{nullptr};
Level Logger::_level{Level::kInfo};

}

#endif // SERIAL_LOGGER_SERIAL_LOGGER_HPP_