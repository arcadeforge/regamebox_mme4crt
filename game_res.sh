vcgencmd hdmi_timings 1920 1 135 224 266 240 1 2 3 16 0 0 0 60.0 0 39854700 1
tvservice -e  "DMT 87" > /dev/null
sleep 0.3
fbset -xres 1920 -yres 240
sleep 0.3
