#---CONFIGURATION---
nodesCount=4
exportDirectory="/export/rpi/node"
localDirectory="fs/node"
interface="eth0"
serverIp="10.0.0.254/24"
#-------------------

function reconfigureNetwork {
	echo "Shutting down iterface "$interface" ..."
	sudo ip link set dev $interface down
	echo "Configuring server ip as "$serverIp" on "$interface
	sudo ip addr add $serverIp dev $interface
	echo "Starting interface "$interface" again ..."
	sudo ip link set dev $interface up
}

function exportNFS {
for node in `seq 1 $nodesCount`;
do
	dirname=$exportDirectory$node
	if [ -e $dirname ]
	then
		echo "Directory already exists "$dirname
	else
		echo "Creating export share: "$dirname
		sudo mkdir -p $dirname
	fi
	echo "Binding local file system to export"
	localdirname=$localDirectory$node
	echo $localdirname" to "$dirname
	sudo mount --bind $localdirname $dirname
done
}

reconfigureNetwork
exportNFS
