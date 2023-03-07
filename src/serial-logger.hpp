#ifndef SERIAL_LOGGER_SERIAL_LOGGER_HPP_
#define SERIAL_LOGGER_SERIAL_LOGGER_HPP_

#include <HardwareSerial.h>
#include "serial-logger-types.hpp"

// Use LOGGER_NO_CONTENT to avoid any print to the console
#if defined(LOGGER_VOID_ALL)
 #include "./private/serial-logger-stub.hpp"
#else 
namespace logging {

/*!
  Logger class maintaining and providing logging resources
*/
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
  /**
   * @brief Configure the GLOBAL log level
   * 
   * @param level The log level
   */
  static void setLevel(Level level);

  /**
   * @brief Provide the serial object used to send the log lines
   * 
   * If you set \a serial to nullptr, logging is disabled.
   * 
   * @param serial The serial instance or nullptr
   */
  static void setSerial(HardwareSerial* serial);

  /**
   * @brief Enable or disable colorization
   * 
   * @param flag Whether or not colorization shall be used
   */
  static void colorize(bool flag);

  /**
   * @brief Check the current colorization setting
   * 
   * @return true if colorization is enabled
   * @return false  if colorization is disabled
   */
  static bool isColorized();

  /**
   * @brief Log some \a data with the provided severity
   * 
   * @tparam T Any type that `HardwareSerial::print(...)` can print
   * @param level The level of the message
   * @param data The data to log
   */
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

  /**
   * @brief Log any message using the c-ish `printf` formatting
   * 
   * @param level The level / severity of the message
   * @param fmt The format following the `printf` logic
   * @param ... The data to fill in the format string
   */
  static void log(Level level, const char * fmt, ...);

  /**
   * @brief Log any data using the `kInfo` log level
   * 
   * @tparam T Any data type supported by `HardwareSerial::print`
   * @param data The data to print
   */
  template<class T>
  static void info(T data){
    auto level = Level::kInfo;
    printId(level);
    printLine<>(data, level);
  };

  /**
   * @brief Log any data on the kInfo level using `printf` logic
   * 
   * @param fmt The format of the text
   * @param ... The data to be added to the format
   */
  static void info(const char * fmt, ...);

  /**
   * @brief Log any data using the `kDebug` log level
   * 
   * @tparam T Any data type supported by `HardwareSerial::print`
   * @param data The data to print
   */
  template<class T>
  static void debug(T line){
    auto level = Level::kDebug;
    printId(level);
    printLine<>(line, level);
  };

  /**
   * @brief Log any data on the kDebug level using `printf` logic
   * 
   * @param fmt The format of the text
   * @param ... The data to be added to the format
   */
  static void debug(const char * fmt, ...);

  /**
   * @brief Log any data using the `kWarning` log level
   * 
   * @tparam T Any data type supported by `HardwareSerial::print`
   * @param data The data to print
   */
  template<class T>
  static void warning(T line){
    auto level = Level::kWarning;
    printId(level);
    printLine<>(line, level);
  };

  /**
   * @brief Log any data on the kWarning level using `printf` logic
   * 
   * @param fmt The format of the text
   * @param ... The data to be added to the format
   */
  static void warning(const char * fmt, ...);

  /**
   * @brief Log any data using the `kError` log level
   * 
   * @tparam T Any data type supported by `HardwareSerial::print`
   * @param data The data to print
   */
  template<class T>
  static void error(T line){
    auto level = Level::kError;
    printId(level);
    printLine<>(line, level);
  };

  /**
   * @brief Log any data on the kError level using `printf` logic
   * 
   * @param fmt The format of the text
   * @param ... The data to be added to the format
   */
  static void error(const char * fmt, ...);

  /**
   * @brief Log any data using the `kError` log level
   * 
   * @tparam T Any data type supported by `HardwareSerial::print`
   * @param data The data to print
   */
  template<class T>
  static void critical(T line){
    auto level = Level::kCritical;
    printId(level);
    printLine<>(line, level);
  };

  /**
   * @brief Log any data on the kError level using `printf` logic
   * 
   * @param fmt The format of the text
   * @param ... The data to be added to the format
   */
  static void critical(const char * fmt, ...);

  /**
   * @brief Write the current log level configuration to the logs
   * 
   * @param level The level used for this message
   */
  static void logLevel(Level level = Level::kInfo);


  private:
  /**
   * @brief Print a line
   */
  template <class T>
  static void printLine(T line, Level level) {
    if (serial_ != nullptr && included_(level) ) {
      serial_->print(line);
      endColorizedSection();
      serial_->println();
    }
  }

  /**
   * @brief Check if the provided log level is should be part of the logs under
   * the given log level configuration
   * 
   * @param level The level to check against the internal setting
   * @return true If \a level is included
   * @return false If \a level is not included
   */
  static bool included_(Level level) noexcept;

  /**
   * @brief Add the level ID to the log file
   * 
   * The ID is only added if we have a valid serial object, and \a level is
   * included in the currently configured level. Starts colorization if
   * configured, too.
   * @param level The level to log.
   */
  static void printId(Level level);

  /**
   * @brief Helper to print the ID
   * 
   * @param id The id to print
   * @param level the level of the message
   * @param fontStyle The style following ANSI code w/o opening and closing
   * elements (valid would be 48;5;2)
   */
  static void printId_(const char * id, Level level, const char * fontStyle);

  /**
   * @brief Write the colorization to the console
   * 
   * The style is only printed if we have a valid serial object, colorization is
   * enabled and style is not nullptr.
   * @param style The style to print following ANSI code w/o opening and
   * closing elements
   */
  static void startColorizedSection(const char * style);

  /**
   * @brief End colorized section by setting back to defaults
   * 
   * Method adds something to the log only if the serial object is valid and
   * colorization is enabled
   */
  static void endColorizedSection();

};

}

#endif // LOGGER_VOID_ALL


#endif // SERIAL_LOGGER_SERIAL_LOGGER_HPP_