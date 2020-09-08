#pragma once

#include <cstdlib>

#define ARRAY_LEN(x) sizeof(x)/sizeof(x[0])

typedef unsigned char uint8;

double randDouble(){
        return rand() / (RAND_MAX + 1.0);
}
