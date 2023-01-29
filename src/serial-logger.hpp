#ifndef SERIAL_LOGGER_SERIAL_LOGGER_HPP_
#define SERIAL_LOGGER_SERIAL_LOGGER_HPP_

#include <HardwareSerial.h>

namespace logging {

enum class Level {
  kWarning,
  kInfo,
  kError,
  kDebug,
  kCritical
};

const char * toString(Level level);

class Logger {
  private:
  static HardwareSerial * serial_;
  static Level _level;

  public:
  static void setLevel(Level level);

  static void setSerial(HardwareSerial* serial);

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

  static void log(Level level, const char * fmt, ...);

  template<class T>
  static void info(T line){
    auto level = Level::kInfo;
    printId("[INFO    ] ", level);
    printLine<>(line, level);
  };

  static void info(const char * fmt, ...);

  template<class T>
  static void debug(T line){
    auto level = Level::kDebug;
    printId("[DEBUG   ] ", level);
    printLine<>(line, level);
  };

  static void debug(const char * fmt, ...);

  template<class T>
  static void warning(T line){
    auto level = Level::kWarning;
    printId("[WARNING ] ", level);
    printLine<>(line, level);
  };

  static void warning(const char * fmt, ...);

  template<class T>
  static void error(T line){
    auto level = Level::kError;
    printId("[ERROR   ] ", level);
    printLine<>(line, level);
  };

  static void error(const char * fmt, ...);

  template<class T>
  static void critical(T line){
    auto level = Level::kCritical;
    printId("[CRITICAL] ", level);
    printLine<>(line, level);
  };

  static void critical(const char * fmt, ...);

  
  static void logLevel(Level level = Level::kInfo);


  private:
  template <class T>
  static void printLine(T line, Level level) {
    if (serial_ != nullptr && included_(level) ) {
      serial_->println(line);
    }
  }

  static void printId(const char * id, Level level);

  static bool included_(Level level) noexcept;

};

}

#endif // SERIAL_LOGGER_SERIAL_LOGGER_HPP_