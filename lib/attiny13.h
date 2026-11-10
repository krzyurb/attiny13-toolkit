#ifndef ATTINY13_H
#define ATTINY13_H

#include <stdbool.h>
#include <stdint.h>

void pinMode(int8_t pin, int8_t mode);
void digitalWrite(int8_t pin, int8_t val);
bool digitalRead(int8_t pin);

#endif
