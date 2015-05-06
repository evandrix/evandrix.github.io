#!/bin/bash

rm -f download.lst
for i in {0..256}; do
	QR=$(cat /dev/urandom | env LC_CTYPE=C tr -cd "A-Za-z0-9" | head -c 20)
	for lvl in 0; do
		echo "http://hackyeaster.hacking-lab.com/hackyeaster/images/egg_${lvl}_${QR}.png" >>download.lst
	done
done
aria2c -q -i download.lst -j 32 -l ""
rm -f download.lst
