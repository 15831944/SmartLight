#!/bin/bash
cd /home/work/ldns/bin
./ldns-manager.sh startMonitor
strp=$(sed -n 2p ldns-program)
echo $strp | sudo -E -s /etc/init.d/avahi-daemon stop
