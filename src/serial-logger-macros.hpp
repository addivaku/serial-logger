#ifndef SERIAL_LOGGER_SERIAL_LOGGER_MACROS_HPP_
#define SERIAL_LOGGER_SERIAL_LOGGER_MACROS_HPP_

#include <stdarg.h>

/**
 * @brief Macro to stringify variable arguments as done in printf, sprintf, etc.
 * 
 * @param fmt The format string argument
 * @param last_arg The last mandatory argument before the optional ones
 * 
 * This method creates a char pointer data which is nullptr if the data could
 * not be stringified. Make sure to call _LOGGER_STRINGIFY_CLEANUP to free
 * allocated space.
 */
#define _LOGGER_STRINGIFY(fmt, last_arg) \
  va_list args; \
  va_start(args, last_arg); \
  \
  char * data = nullptr; \
  auto estimatedSize = vsnprintf(nullptr, 0, fmt, args);  \
  if (estimatedSize >= 0) { \
    const auto kSize = estimatedSize + 5; \
    data = new char[kSize]; \
    \
    vsnprintf(data, kSize, fmt, args); \
  }

/**
 * @brief Clean up after variable arguments were stringified and USED.
 * 
 * Note that the data variable is freed and set to nullptr in this macro.
 * 
 */
#define _LOGGER_STRINGIFY_CLEANUP() \
  delete [] data; \
  data = nullptr; \
  va_end(args);   \

#endif // SERIAL_LOGGER_SERIAL_LOGGER_MACROS_HPP_