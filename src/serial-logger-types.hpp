#ifndef SERIAL_LOGGER_TYPES_HPP_
#define SERIAL_LOGGER_TYPES_HPP_

namespace logging {

//! Enumeration of all log levels
enum class Level {
  kWarning,
  kInfo,
  kError,
  kDebug,
  kCritical
};

/*!
  Utility function to convert \a Level enumerators to printable char arrays

  \param level The enumerator to convert
  \returns A pointer to the converted description.
*/
const char * toString(Level level);

} // namespace logging

#endif // SERIAL_LOGGER_TYPES_HPP_