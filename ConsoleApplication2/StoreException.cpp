#include "StoreException.h"


StoreException::StoreException(const ExceptionType t, const std::string& msg) : runtime_error(msg), type(t) { }
