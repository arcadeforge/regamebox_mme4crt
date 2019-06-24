This is a nice tool for the regamebox distribution 
made for the pi2scart and pi2jamma hardware. 

Get the hardware at 

http://www.arcadeforge.de


This tool generates a raspberry pi conform hdmi_timings for retro games.

Input Paramter

width - width resolution of game  
height - height resolution of game
freq - frequency of game
shift - x shift on screen
superres - 0 or 1 for superres use and not

Build

gcc -o mme4crt mme4crt.c

Usage
./mme4crt 320 224 60 0 0  

