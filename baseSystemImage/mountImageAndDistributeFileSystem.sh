#---CONFIGURATION---
systemPartitionOffset=$(expr 512 \* 97728)
nodesCount=1
nodesDirectoryPrefix="../networkFileSystem/fs/node"
#-------------------

echo $systemPartitionOffset

sudo mkdir /mnt/pi-root-loop
sudo mount -o loop,offset=$systemPartitionOffset 2015-02-18-wheezy-minibian.img /mnt/pi-root-loop
ls /mnt/pi-root-loop
for node in `seq 1 $nodesCount`;
do
	sudo cp -rav /mnt/pi-root-loop/* $nodesDirectoryPrefix$node
done