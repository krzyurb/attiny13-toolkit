# attiny13-toolkit

Personal toolkit for building ATtiny13A firmware

## Install

**Linux**

```sh
$ sudo apt-get install avrdude gcc-avr
```

**MacOs**

```sh
$ brew trust osx-cross/avr
$ brew tap osx-cross/avr
$ brew install avr-gcc avrdude
```

## Projects

Toolkit unifies building and loading code to AVR microcontroller. Each project consist of:

* `main.c` - entrypoint for code
* `./out` - dir with build results

## Usage

* `make status` - check if bootloader is connected properly to the unit
* `make build PROJECT=project_name` - builds project and generates hex file
* `make deploy PROJECT=project_name` - loads code to the unit
* `make new-project PROJECT=project_name` - creates new project using template
