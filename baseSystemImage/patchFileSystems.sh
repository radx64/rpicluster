#---CONFIGURATION---
nodesCount=4
nodesDirectoryPrefix="../networkFileSystem/fs/node"
#-------------------

modifyFSTAB() {
	echo "/dev/mmcblk0p1 /boot vfat defaults 0 2" > fstab
	echo "filesystem:/mirror /mirror nfs" >> fstab
	echo "#/dev/mmcblk0p2 / ext4 errors=remount-ro,noatime 0 1" >> fstab
	chmod 0644 fstab
	mv -f fstab $1
}

modifyNetworking() {
	echo "auto lo" > interfaces
	echo "iface lo inet loopback" >> interfaces
	echo "auto eth0" >> interfaces
	echo "iface eth0 inet static" >> interfaces
	echo "address 192.168.0."$1 >> interfaces
	echo "netmask 255.255.255.0" >> interfaces
	echo "gateway 192.168.0.254" >> interfaces
	chmod 0644 interfaces
	mv -f interfaces $2
}

modifyRCS() {
	echo "ASYNCMOUNTNFS=no" >> $1
}

modifyHostname() {
	echo "node"$1 > $2
}

for node in `seq 1 $nodesCount`;
do
	echo -e " \e[92mPatching node"$node" filesystem \e[39m... "
	fstabLocation=$nodesDirectoryPrefix$node"/etc/fstab"
	modifyFSTAB $fstabLocation
	interfacesLocation=$nodesDirectoryPrefix$node"/etc/network/"
	modifyNetworking $node $interfacesLocation
	modifyRCS $nodesDirectoryPrefix$node"/etc/default/rcS"
	modifyHostname $node $nodesDirectoryPrefix$node"/etc/hostname"
done