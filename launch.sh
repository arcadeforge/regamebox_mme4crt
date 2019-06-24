#!/usr/bin/bash

#vcgencmd hdmi_timings 2035 1 155 238 422 224 1 10 3 24 0 0 0 60.000000 0 44631000.000000 1
#tvservice -e  "DMT 87" > /dev/null
#sleep 0.3
#fbset -depth 8 && fbset -depth 24
#sleep 0.3
retroarch -L /root/.config/retroarch/cores/mame2003_plus_libretro.so "/mnt/sda/rpi2jamma/roms/arcade/fshark".zip > /dev/null 2>launch.log 
