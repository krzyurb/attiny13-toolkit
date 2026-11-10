# Morse I/O

## Schema

**Components**

- ATtiny13A microcontroller
- 470 Ω resistor x2
- LED light
- Button x2

```
                    +5V
                 ┌────┴────┐          Red
                 │ ATtiny13│          LED
  GND ─[BUTTON]──│PB3   PB1├──[470R]──|>|── GND
  GND ─[BUTTON]──│PB4   PB0├──[470R]──|>|── GND
                 │         │          Green
                 └────┬────┘          LED
                     GND
```

## Data structure


### Letter byte

**Schema**
```
76543210 -> index
01100101 -> letter
|   |
|   |
|   └ dit/dah -> "_._" -> "e"
|
└ length bits -> 3
```

**Examples**
* `01100101` -> e (size: 3)
* `10001011` -> y (size: 4)
* `00000000` -> space (size: 0)

### Timing

* `.` -> 1u
* `_` -> 3u
* letter space -> 3u
* word space -> 6u
