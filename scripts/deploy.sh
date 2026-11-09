#!/bin/bash

avrdude \
  -c usbasp \
  -p t13 \
  -vvv \
  -U flash:w:"projects/$1/out/main.hex":i
