#include "serial-logger.hpp"
#include "serial-logger-macros.hpp"

#if !defined(LOGGER_VOID_ALL)
namespace logging {

Logger::Logger(const char *name) : 
    serial_{nullptr},
    level_ {Level::kInfo},
    colorize_{false} {
  name_ = new char[ strlen(name) + 1];
  strcpy(name_, name);
}

Logger::Logger() : Logger("global") {

}

Logger::~Logger() {
  delete [] name_;
}

Logger& Logger::setLevel(Level level) {
  level_ = level;
  return *this;
}

Logger& Logger::setSerial(HardwareSerial * serial) {
  serial_ = serial;
  return *this;
}

Logger& Logger::colorize(bool enabled) {
  colorize_ = enabled;
  return *this;
}

bool Logger::isColorized() {
  return colorize_;
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
  Logger::log(level, "Log level is: %s", toString(level_) );
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

  printId_(id, level, format);
}

void Logger::printId_(const char * id, Level level, const char * style) {
  if (serial_ != nullptr && included_(level)) {
    // determine size of target string
    // auto format = "[\033[%sm%-*s\033[0m] ";
    startColorizedSection(style);
    // avr-gcc fails on variable width precision like %-*s. To work around this
    // issue, we add a seperate buffer
    const char kFormatTemplate[]= "{%%s} - [%%-%us] ";
    const size_t kFormatSize = sizeof(kFormatTemplate) + 1U;
    char format[kFormatSize];
    snprintf(format, kFormatSize, kFormatTemplate, kIdFieldSize);
    
    const auto size = snprintf(nullptr, 0, format, name_, id) + 1;

    auto buffer = new char[size];
    snprintf(buffer, size, format, name_, id);
    serial_->print(buffer);
    delete [] buffer;
  } 
}

bool Logger::included_(Level level) noexcept {
  if (level_ == Level::kDebug) return true; // debug contains everything
  if (level_ == Level::kInfo) {
    // print everything but debug
    return level != Level::kDebug;
  }
  if (level_ == Level::kWarning) {
    // print kError, kWarning, kCritical
    return (   level == Level::kWarning 
            || level == Level::kError
            || level == Level::kCritical);
  }
  if (level_ == Level::kError) {
    // print kError and kCritical
    return (level == Level::kError || level == Level::kCritical);
  }
  if (level_ == Level::kCritical) return level == Level::kCritical;
  return false;
}

void Logger::startColorizedSection(const char * fmt) {
  if (serial_ && colorize_ && fmt) {
    auto colorTemplate = "\033[%sm";
    const auto size = snprintf(nullptr, 0, colorTemplate, fmt);

    if (size > 0) {
      char * buffer = new char[size + 1];

      snprintf(buffer, size + 1, colorTemplate, fmt);
      serial_->print(buffer);

      delete [] buffer;
    }
  }
}

void Logger::endColorizedSection() {
  if (serial_ && colorize_) {
    serial_->print("\033[0m");
  }
}

const char * Logger::kDebugFormat {"38;5;12"};
const char * Logger::kInfoFormat {"38;5;10"};
const char * Logger::kWarningFormat {"38;5;11"};
const char * Logger::kErrorFormat {"38;5;9"};
const char * Logger::kCriticalFormat {"97;41"};
const char Logger::kIdFieldSize {8};

}

#endif // !defined(LOGGER_VOID_ALL)