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
mode - 0, 1, 2 for modes
  mode = 0 write result file on disk
  mode = 1 write results in files for regamebox
  mode = 2 execute timings directly

Build

gcc -o mme4crt mme4crt.c

Usage

./mme4crt 320 224 60 5 0 2

execute res of 320@224@60, move pic 5 pix right, no superres and execute res directly


./mme4crt 320 224 60 -30 1 1  

execute super res of 320@224@60, move pic 30 pix left, use superres and write files needed for regamebox


Result files

game_res.sh
- contains scripts for changing resolutions

retroarch_game.cfg
- contains config for retroarch

timings.txt
- sort of logfile


