#include <iostream>
#include <set>
#include "SetUtils.h"

using namespace std;

void set_delete(set<int>* s) {
    s->erase(s->begin(), s->end());
}

void set_delete(set<string>* s) {
    s->erase(s->begin(), s->end());
}