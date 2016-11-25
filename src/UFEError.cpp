#include "UFEError.h"

UFEError::UFEError(const std::string &ErrorDescr,
                       const std::string &ErrorLocation,
                       int Svr)
: errorDescription_(ErrorDescr), errorLocation_(ErrorLocation), severity_(Svr) {}

std::string UFEError::GetDescription() {
    return "*** " + errorDescription_ + " ***";
}

std::string UFEError::GetLocation() {
  return SeverityToString() + " in " + errorLocation_;
}

std::string UFEError::SeverityToString() {
  std::string s;
  switch (severity_) {
    case WARNING:
      s = "WARNING";
      break;

    case SERIOUS:
      s = "SERIOUS ERROR";
      break;

    case FATAL:
      s = "FATAL ERROR";
      break;
  }
  return s;
}


