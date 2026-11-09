# Hello World

Kind of hello world with blinking led light.

## Schema

Components:

- ATtiny13A microcontroller
- 470 Ω resistor
- LED light

```
      +5V
  ┌────┴────┐
  │ ATtiny13│
  │         │
  │      PB0├──[470R]──|>|── GND
  │         │          LED
  └────┬────┘
      GND
```
