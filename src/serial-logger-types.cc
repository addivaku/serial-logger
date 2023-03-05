#include "serial-logger-types.hpp"

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


}