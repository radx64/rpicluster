#!/bin/bash
HOSTS=4

while true;
do

for node in `seq 1 $HOSTS`
do
  count=$(ping -c 1 192.168.0.$node | grep 'received' | awk -F',' '{ print $2 }' | awk '{ print $1 }')
  if [ $count -eq 0 ]; then
    STATUSES[$node]="\e[91m  FAIL  \e[39m"
  else
  	STATUSES[$node]="\e[92m   OK   \e[39m"
  fi
done

clear

for host in ${!STATUSES[*]}
do
	echo -e "192.168.0."$host" - " ${STATUSES[$host]} 
done
sleep 1
done