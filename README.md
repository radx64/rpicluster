# rpicluster
Cluster based on 4xRPi

[![Build Status](https://travis-ci.org/radx64/rpicluster.svg?branch=master)](https://travis-ci.org/radx64/rpicluster)

![Rpis in cluster](https://img.shields.io/badge/rpi%20count-4-green.svg)

![Peak power usage](https://img.shields.io/badge/peak%20power%20usage-42%20W-green.svg)

Cluster will be based on four Rasperry Pi computers and network switch.

##### Things to do first:
- [x] Connect step-up step-down converter to lower voltage from 12V to 7.5V needed to power network switch.
- [x] Prepare SD Cards with UBoot only.
- [x] Configure NFS server.
- [x] Prepare Images with Minibian distribution.
- [x] Install and configure MPICH2
- [ ] Run some distributed computing test.

##### Different aproach:
- [x] Find some HA software
- [ ] [Ongoing] Configure  it
- [ ] Make some tests (python script to poll mysql database)
