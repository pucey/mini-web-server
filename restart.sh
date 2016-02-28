#!/bin/sh
killall -9 final
make
./final -d /home/lyr1k/Desktop/mini-web-server/root-webserver -h 127.0.0.1 -p 12345
ps aux | grep final
