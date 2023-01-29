#include "serial-logger.hpp"
#include "serial-logger-macros.hpp"

namespace logging {

const char * toString(Level level)  {
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

void Logger::setLevel(Level level) {
  _level = level;
}

void Logger::setSerial(HardwareSerial * serial) {
  serial_ = serial;
}


void Logger::log(Level level, const char * fmt, ...) {
  if (serial_) {
    _LOGGER_STRINGIFY(fmt, fmt);
    if (data) 
      log<>(level, data);
    _LOGGER_STRINGIFY_CLEANUP();
  }
}

void Logger::info(const char * fmt, ...) {
  if (serial_) {
    _LOGGER_STRINGIFY(fmt, fmt);
    if (data)
      info<>(data);
    _LOGGER_STRINGIFY_CLEANUP();
  }
}

void Logger::debug(const char * fmt, ...)  {
  if (serial_) {
    _LOGGER_STRINGIFY(fmt, fmt);
    if (data)
      debug<>(data);
    _LOGGER_STRINGIFY_CLEANUP();
  }
}

void Logger::warning(const char * fmt, ...) {
  if (serial_) {
  _LOGGER_STRINGIFY(fmt, fmt);
  if (data)
    warning<>(data);
  _LOGGER_STRINGIFY_CLEANUP();
  }
}

void Logger::error(const char * fmt, ...) {
  if (serial_) {
    _LOGGER_STRINGIFY(fmt, fmt);
    if (data)
      error<>(data);
    _LOGGER_STRINGIFY_CLEANUP();
  }
}

void Logger::critical(const char * fmt, ...) {
  if (serial_) {
    _LOGGER_STRINGIFY(fmt, fmt);
    if (data)
      critical<>(data);
    _LOGGER_STRINGIFY_CLEANUP();
  }
}

void Logger::logLevel(Level level) {
  Logger::log(level, "Log level is: %s", toString(level) );
}

void Logger::printId(const char * id, Level level) {
  if (serial_ != nullptr && included_(level) ) {
    serial_->print(id);
  }
}

bool Logger::included_(Level level) noexcept {
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

HardwareSerial* Logger::serial_{nullptr};
Level Logger::_level{Level::kInfo};

}
