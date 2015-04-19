#---CONFIGURATION---
systemPartitionOffset=$(expr 512 \* 97728)
nodesCount=4
nodesDirectoryPrefix="../networkFileSystem/fs/node"
#-------------------

echo $systemPartitionOffset

sudo mkdir /mnt/pi-root-loop
sudo mount -o loop,offset=$systemPartitionOffset system.img /mnt/pi-root-loop
ls /mnt/pi-root-loop
for node in `seq 1 $nodesCount`;
do	
	mkdir $nodesDirectoryPrefix$node
	sudo cp -rav /mnt/pi-root-loop/* $nodesDirectoryPrefix$node
done