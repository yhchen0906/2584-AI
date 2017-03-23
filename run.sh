#!/bin/sh
rate=$( grep "ALPHA =" Fib2584Ai.h | awk '{print $5}' | sed 's/;//' )
echo "***********************"
date +"%m/%d %H:%M $rate"
echo
./play_game 200000
echo
date +"%m/%d %H:%M"
