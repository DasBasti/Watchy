#!/bin/bash
DATE=$(echo -n `date +%Y-%m-%dT%H:%M:%S` | xxd -ps | sed 's/[[:xdigit:]]\{2\}/&/g')
echo $DATE
sudo gatttool -i hci0 -b 94:B9:7E:89:23:B2 --char-write-req -a 0x0040 -n $DATE