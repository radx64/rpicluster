# rpicluster
Cluster based on 4xRPi

![Rpis in cluster](https://img.shields.io/badge/rpi%20count-4-green.svg)

![Peak power usage](https://img.shields.io/badge/peak%20power%20usage-36%20W-green.svg)
![Compute power MIPS](https://img.shields.io/badge/compute%20power-UNKNW%20MIPS-red.svg)
![Compute power FLOPS](https://img.shields.io/badge/compute%20power-UNKNW%20FLOPS-red.svg)


Cluster will be based on four Rasperry Pi computers and network switch.

##### Things to do first:
- [x] Connect step-up step-down converter to lower voltage from 12V to 7.5V needed to power network switch.
- [ ] Prepare SD Cards with UBoot only.
- [-] Configure DHCP server. (at begining only static network config)
- [-] Configure TFTP server. (using NFS instead)
- [ ] Configure NFS server.
- [ ] Prepare Images with Minibian distribution.
