#!/bin/bash

openocd -f ../stm32f103.cfg -c "program bin/a.bin reset run exit 0x08000000"
