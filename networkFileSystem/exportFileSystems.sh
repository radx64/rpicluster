#---------------------------
nodesCount=4
exportDirectory="/export/rpi/node"
localDirectory="fs/node"
#---------------------------

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


#sudo mount --bind fs/node /export/users