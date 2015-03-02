#!/bin/bash

zip -Zb system.zip $1 -v
split -d -b48m system.zip system.zip
rm system.zip
rm $1