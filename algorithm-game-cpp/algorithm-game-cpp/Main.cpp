#include <iostream>
#include <chrono>
#include "OneLine.h"

long long get_time() {
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    );
    return ms.count();
}

int main() {
    long long startTime = get_time();
    std::cout << to_string(startTime) << std::endl;
    char var1[] = "0,18,30,68,44,0,10";
    char* var2 = new char[2048];
    one_line_main(var1, var2);
    std::cout << to_string(get_time() - startTime) << std::endl;
    system("pause");
}

