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
    long long startTime = get_time();
    std::cout << to_string(startTime) << std::endl;
    one_stroke_main();
    std::cout << to_string(get_time() - startTime) << std::endl;
    system("pause");
}

