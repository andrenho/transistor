#ifndef EXCEPTIONS_HH
#define EXCEPTIONS_HH

#include <stdexcept>

class RecoverableErrorOptional : public std::runtime_error {
public:
    explicit RecoverableErrorOptional(const char* what) : std::runtime_error(what) {}
};

#endif //EXCEPTIONS_HH
