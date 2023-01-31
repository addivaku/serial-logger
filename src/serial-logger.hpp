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
  static bool _colorize;

  static const char * kDebugFormat;
  static const char * kInfoFormat;
  static const char * kWarningFormat;
  static const char * kErrorFormat;
  static const char * kCriticalFormat;
  static const char kIdFieldSize;

  public:
  static void setLevel(Level level);

  static void setSerial(HardwareSerial* serial);

  static void colorize(bool flag);

  static bool isColorized();

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
    printId(level);
    printLine<>(line, level);
  };

  static void info(const char * fmt, ...);

  template<class T>
  static void debug(T line){
    auto level = Level::kDebug;
    printId(level);
    printLine<>(line, level);
  };

  static void debug(const char * fmt, ...);

  template<class T>
  static void warning(T line){
    auto level = Level::kWarning;
    printId(level);
    printLine<>(line, level);
  };

  static void warning(const char * fmt, ...);

  template<class T>
  static void error(T line){
    auto level = Level::kError;
    printId(level);
    printLine<>(line, level);
  };

  static void error(const char * fmt, ...);

  template<class T>
  static void critical(T line){
    auto level = Level::kCritical;
    printId(level);
    printLine<>(line, level);
  };

  static void critical(const char * fmt, ...);

  
  static void logLevel(Level level = Level::kInfo);


  private:
  template <class T>
  static void printLine(T line, Level level) {
    if (serial_ != nullptr && included_(level) ) {
      serial_->print(line);
      endColorizedSection();
      serial_->println();
    }
  }
  static void printId(Level level);

  static void printId_(const char * id, Level level, const char * fontStyle);

  static bool included_(Level level) noexcept;

  static void startColorizedSection(const char * format);

  static void endColorizedSection();

};

}

#endif // SERIAL_LOGGER_SERIAL_LOGGER_HPP_