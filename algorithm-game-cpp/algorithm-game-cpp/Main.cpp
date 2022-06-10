#include <iostream>
#include <chrono>
#include "OneStroke.h"

long long get_time() {
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    );
    return ms.count();
}

int main() {
    std::cout << get_time() << std::endl;
    OneStroke_Main();
    std::cout << "Hello World!\n";
}

