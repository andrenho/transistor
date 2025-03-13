#ifndef TESTS_HH
#define TESTS_HH

class Tests {
public:
    static void run_tests();

private:
    static void test_native_query();
    static void test_lua_engine();
    static void test_simulation();
};

#endif //TESTS_HH
