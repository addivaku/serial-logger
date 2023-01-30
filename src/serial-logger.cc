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

void Logger::colorize(bool enabled) {
  _colorize = enabled;
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

void Logger::printId(Level level) {
  const char * id = nullptr;
  const char * format = nullptr;
  switch (level) {
    case Level::kDebug:
      id = "DEBUG";
      format = kDebugFormat;
      break;
    case Level::kInfo:
      id = "INFO";
      format = kInfoFormat;
      break;
    case Level::kWarning:
      id = "WARNING";
      format = kWarningFormat;
      break;
    case Level::kError:
      id = "ERROR";
      format = kErrorFormat;
      break;
    case Level::kCritical:
      id = "CRITICAL";
      format = kCriticalFormat;
      break;
    default:
      id = "UNDEF";
      format = "0";
  }

  if (_colorize) {
    printId_(id, level, format);
  } else {
    printId_(id, level);
  }
}

void Logger::printId_(const char * id, Level level) {
  if (serial_ != nullptr && included_(level) ) {
    const auto kWhitespaceLength = 8;
    const auto kBufferLength = kWhitespaceLength + 2U + 1U + 1U;
    char * buffer = new char[kBufferLength];
    snprintf(buffer, kBufferLength, "[%-*s] ", kWhitespaceLength, id);
    serial_->print(buffer);
    delete [] buffer;
  }
}



void Logger::printId_(const char * id, Level level, const char * style) {
  if (serial_ != nullptr && included_(level)) {
    // determine size of target string
    auto format = "[\033[%sm%-*s\033[0m] ";
    const auto size = snprintf(nullptr, 0, format,  style, 8, id) + 1;

    auto buffer = new char[size];
    snprintf(buffer, size, format, style, 8, id);
    serial_->print(buffer);
    delete [] buffer;
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
bool Logger::_colorize {false};
const char * Logger::kDebugFormat {"38;5;12"};
const char * Logger::kInfoFormat {"38;5;10"};
const char * Logger::kWarningFormat {"38;5;11"};
const char * Logger::kErrorFormat {"38;5;9"};
const char * Logger::kCriticalFormat {"48;5;9"};

}
