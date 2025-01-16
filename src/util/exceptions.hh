#ifndef EXCEPTIONS_HH
#define EXCEPTIONS_HH

#include <stdexcept>

class RecoverableException : public std::runtime_error {
public:
    explicit RecoverableException(const char* what) : std::runtime_error(what) {}
};

#endif //EXCEPTIONS_HH
