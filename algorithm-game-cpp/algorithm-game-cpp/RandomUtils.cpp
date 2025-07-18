#include<stdlib.h> 
#include<time.h>

int ran_int(int minValue, int maxValue) {
    srand(time(nullptr));
    return minValue + rand() % (maxValue - minValue);
}