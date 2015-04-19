#---CONFIGURATION---
nodesCount=4
exportDirectory="/export/rpi/node"
localDirectory="fs/node"
interface="eth0"
serverIp="192.168.0.254/24"
networkIp="192.168.0.0/24"
#-------------------

function welcome {
	echo -e "========== \e[92mNFS shares configuration script \e[39m========="
	echo -e "Configuration that will be applied:"
	echo -e "\t Export directory prefix: \e[92m"$exportDirectory"\e[39m"
	echo -e "\t Local directory prefix:  \e[92m"$localDirectory"\e[39m"
	echo -e "\t Server interface:        \e[92m"$interface"\e[39m"
	echo -e "\t Server IP:               \e[92m"$serverIp"\e[39m"
	echo -e "\t Network IP:              \e[92m"$networkIp"\e[39m"

	echo -e "\e[91mWarning!. Your actual /etc/exports configuration will be overwritten!"
	echo -e "\e[39m"
}

function pause(){
   read -p "$*"
}


function reconfigureNetwork {
	echo "Shutting down iterface "$interface" ..."
	sudo ip link set dev $interface down
	echo "Configuring server ip as "$serverIp" on "$interface
	sudo ip addr add $serverIp dev $interface
	echo "Starting interface "$interface" again ..."
	sudo ip link set dev $interface up
}

function exportNFS {
echo "/mirror *(rw,sync)" > config
for node in `seq 1 $nodesCount`;
do
	localdirname=$localDirectory$node
	if [ -e $localdirname ]
	then
		echo "Directory already exists "$localdirname
	else
		echo "Creating local filedir: "$localdirname
		sudo mkdir -p $localdirname
		sudo chmod 777 $localdirname
	fi

	dirname=$exportDirectory$node
	if [ -e $dirname ]
	then
		echo "Directory already exists "$dirname
	else
		echo "Creating export share: "$dirname
		sudo mkdir -p $dirname
		sudo chmod 777 $dirname
	fi
	echo "Binding local file system to export"
	
	echo $localdirname" to "$dirname
	sudo mount --bind $localdirname $dirname

	echo $exportDirectory$node" "$networkIp"(rw,no_root_squash,no_subtree_check)" >> config
done
sudo mv config /etc/exports
sudo service nfs-kernel-server restart
}

welcome
pause "Press [Enter] to continue configuration... [CTRL+C] to abort."
reconfigureNetwork
exportNFS
