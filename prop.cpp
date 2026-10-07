#include <iostream>

#include "value.h"
#include "test.h"

int main(){
    run_tests();

    Value a(2.0);
    Value b(-3.0);
    Value c = a * b + Value(10).relu();
    c.backward();
    std::cout << a.grad() << std::endl;
}
