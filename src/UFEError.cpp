#include "UFEError.h"

using namespace std;

UFEError::UFEError(const string &ErrorDescr,
                       const string &ErrorLocation,
                       int Svr)
: errorDescription_(ErrorDescr), errorLocation_(ErrorLocation), severity_(Svr) {}

string UFEError::getDescription() {
    return "*** " + errorDescription_ + " ***";
}

string UFEError::getLocation() {
  return severityToString() + " in " + errorLocation_;
}

int UFEError::getSeverity() {
  return severity_;
}

string UFEError::severityToString() {
  string s;
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

