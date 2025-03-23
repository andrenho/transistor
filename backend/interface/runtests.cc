#include "transistor.hh"

int main()
{
    Transistor t;
    return t.run_tests().first ? 0 : 127;
}