#!/bin/bash

rm -f projects/$1/out/main.elf
rm -f projects/$1/out/main.hex

avr-gcc \
  -mmcu=attiny13a \
  -Os -std=c11 \
  -DF_CPU=1200000UL \
  -o projects/$1/out/main.elf projects/$1/main.c

avr-objcopy -O ihex -R .eeprom projects/$1/out/main.elf projects/$1/out/main.hex
