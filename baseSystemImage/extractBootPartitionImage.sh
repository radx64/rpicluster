#---CONFIGURATION---
bootPartitionOffset=16
bootPartitionSize=97712
imageName="2015-02-18-wheezy-minibian.img"
#-------------------

dd if=$imageName of="boot.img" skip=$bootPartitionOffset bs=512 count=$bootPartitionSize