#!/bin/bash

# openocd -f /usr/share/openocd/scripts/board/stm3210b_eval.cfg -c "program bin/a.bin reset run exit 0x08000000"
openocd -f ../stm32f407.cfg -c "program bin/a.bin reset run exit 0x08000000"
