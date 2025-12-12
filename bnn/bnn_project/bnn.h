#ifndef BNN_H
#define BNN_H
#include <stdint.h>

typedef int16_t DTYPE;
typedef int16_t ITYPE;

void bnn(DTYPE IN[784], ITYPE ys[10]);

#endif
