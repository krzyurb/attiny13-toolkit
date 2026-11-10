#include <avr/io.h>
#include "attiny13.h"

#define OUTPUT 0
#define INPUT 1

#define HIGH 1
#define LOW 0

void pinMode(int8_t pin, int8_t mode)
{
  if (mode == OUTPUT)
  {
    DDRB |= (1 << pin);
  }

  if (mode == INPUT)
  {
    DDRB &= ~(1 << pin);
    PORTB |= (1 << pin);
  }
}

void digitalWrite(int8_t pin, int8_t val)
{
  if (val == HIGH) {
    PORTB |= (1 << pin);
  }

  if (val == LOW) {
    PORTB &= ~(1 << pin);
  }
}

bool digitalRead(int8_t pin)
{
  return !(PINB & (1 << pin));
}
