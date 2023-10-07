#ifndef SERIAL_LOGGER_SERIAL_LOGGER_STUB_HPP_
#define SERIAL_LOGGER_SERIAL_LOGGER_STUB_HPP_

#include <HardwareSerial.h>
#include "serial-logger-types.hpp"

#if defined(LOGGER_VOID_ALL)

namespace logging {

/*!
  Logger class maintaining and providing logging resources
*/
class Logger {
  public:
  /**
   * @brief Construct a new Logger object
   * 
   */
  Logger() = default;

  /**
   * @brief Construct a new Logger object
   * 
   */
  Logger(const char *) : Logger(){};

  /**
   * @brief Destroy the Logger object
   * 
   */
  virtual ~Logger() = default;

  /**
   * @brief Configure the GLOBAL log level
   * 
   * @param level The log level
   */
  static void setLevel(Level /* level */ ) {}

  /**
   * @brief Provide the serial object used to send the log lines
   * 
   * If you set \a serial to nullptr, logging is disabled.
   * 
   * @param serial The serial instance or nullptr
   */
  static void setSerial(HardwareSerial* /* serial */) { }

  /**
   * @brief Enable or disable colorization
   * 
   * @param flag Whether or not colorization shall be used
   */
  static void colorize(bool /* flag */) { }

  /**
   * @brief Check the current colorization setting
   * 
   * @return true if colorization is enabled
   * @return false  if colorization is disabled
   */
  static bool isColorized() { return false; }

  /**
   * @brief Log some \a data with the provided severity
   * 
   * @tparam T Any type that `HardwareSerial::print(...)` can print
   * @param level The level of the message
   * @param data The data to log
   */
  template <class T>
  static void log(Level /* level */, T /* data */) { }

  /**
   * @brief Log any message using the c-ish `printf` formatting
   * 
   * @param level The level / severity of the message
   * @param fmt The format following the `printf` logic
   * @param ... The data to fill in the format string
   */
  static void log(Level /* level */ , const char * /* fmt */, ...) { }

  /**
   * @brief Log any data using the `kInfo` log level
   * 
   * @tparam T Any data type supported by `HardwareSerial::print`
   * @param data The data to print
   */
  template<class T>
  static void info(T /* data*/ ){ }

  /**
   * @brief Log any data on the kInfo level using `printf` logic
   * 
   * @param fmt The format of the text
   * @param ... The data to be added to the format
   */
  static void info(const char * /* fmt */, ...) {}

  /**
   * @brief Log any data using the `kDebug` log level
   * 
   * @tparam T Any data type supported by `HardwareSerial::print`
   * @param data The data to print
   */
  template<class T>
  static void debug(T /* line */){ };

  /**
   * @brief Log any data on the kDebug level using `printf` logic
   * 
   * @param fmt The format of the text
   * @param ... The data to be added to the format
   */
  static void debug(const char * /* fmt */ , ...) { }

  /**
   * @brief Log any data using the `kWarning` log level
   * 
   * @tparam T Any data type supported by `HardwareSerial::print`
   * @param data The data to print
   */
  template<class T>
  static void warning(T /* line */){ };

  /**
   * @brief Log any data on the kWarning level using `printf` logic
   * 
   * @param fmt The format of the text
   * @param ... The data to be added to the format
   */
  static void warning(const char * /* fmt */, ...) {}

  /**
   * @brief Log any data using the `kError` log level
   * 
   * @tparam T Any data type supported by `HardwareSerial::print`
   * @param data The data to print
   */
  template<class T>
  static void error(T /* line */ ){ };

  /**
   * @brief Log any data on the kError level using `printf` logic
   * 
   * @param fmt The format of the text
   * @param ... The data to be added to the format
   */
  static void error(const char * /* fmt */ , ...) { }

  /**
   * @brief Log any data using the `kError` log level
   * 
   * @tparam T Any data type supported by `HardwareSerial::print`
   * @param data The data to print
   */
  template<class T>
  static void critical(T /* line */){ };

  /**
   * @brief Log any data on the kError level using `printf` logic
   * 
   * @param fmt The format of the text
   * @param ... The data to be added to the format
   */
  static void critical(const char * /* fmt */, ...) { }

  /**
   * @brief Write the current log level configuration to the logs
   * 
   * @param level The level used for this message
   */
  static void logLevel(Level /* level */) { }
};

}

#endif //LOGGER_VOID_ALL

#endif // SERIAL_LOGGER_SERIAL_LOGGER_STUB_HPP_