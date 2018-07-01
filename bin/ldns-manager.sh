#!/bin/bash

case $1 in
'start')
	bash ldns-shutdown
	sleep 2
	bash ldns-start
	;;
'stop')
	bash ldns-shutdown
	;;
'monitor')
	bash ldns-monitor > /dev/null 2>&1 &
	echo "ldns-monitor"
	;;
'startMonitor')
  echo "ldns-start"
	bash ldns-start
	echo "ldns-monitor"
	bash ldns-monitor > /dev/null 2>&1 &
	;;
'stopMonitor')
	bash ldns-stopMonitor
	;;
*)
	echo "usage: $0 {start|stop|startMonitor|stopMonitor}"
	echo "params function:"
	echo "start: start programs"
	echo "stop: stop program, but can not stop programs started by startAndMonitor"
	echo "startMonitor: start programs and monitor programs"
	echo "stopMonitor: stop monitor,but do not stop program"
	echo "monitor: monitor programs"
	;;
esac
exit 0

