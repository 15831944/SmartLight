#!/bin/bash
program=RtdbSave
number=`ps -ef|grep $program|grep -v grep|wc -l`
if [ $number -ge 1 ] ; then
	pid=`ps -ef|grep $program|grep -v grep|awk '{print $2}'`;
	if kill -9 $pid ; then
		echo "$program stopped"
	else
		echo "Unable to stop $program"
	fi
fi



