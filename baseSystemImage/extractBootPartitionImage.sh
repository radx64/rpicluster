#---CONFIGURATION---
bootPartitionOffset=16
bootPartitionSize=97712
imageName="system.img"
#-------------------

dd if=$imageName of="boot.img" skip=$bootPartitionOffset bs=512 count=$bootPartitionSize