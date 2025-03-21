#include "transistor.hh"

int main()
{
    Transistor t;
    return t.run_tests() ? 0 : 127;
}