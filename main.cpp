#include "EnvHelper.hpp"
#include <iostream>

int main() {
    std::cout << EnvHelperWithDefault<"PATH","teee",false>::get() << std::endl;
    std::cout << EnvHelperWithDefault<"PATH",'a' ,false>::get() << std::endl;
    std::cout << EnvHelperWithDefault<"PATH",123.4,false>::get() << std::endl;
    std::cout << EnvHelperWithDefault<"PATH2",123,false>::get() << std::endl;
    return 0;
}