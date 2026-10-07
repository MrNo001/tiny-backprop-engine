#include <iostream>

#include "test.h"
#include "value.h"

void run_tests() {
    Value a(2.0);
    Value b(-3.0);
    Value c(10.0);

    Value d = a * b + c;          // 4
    Value e = d.pow(2) / a;       // 8
    Value f = (e - b).relu();     // 11
    Value g = -f + 2.0 * c * a;       // 9
    g.backward();

    std::cout << "g = " << g << std::endl;
    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    std::cout << "c = " << c << std::endl;
    std::cout << "d = " << d << std::endl;
}
