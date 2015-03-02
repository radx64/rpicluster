#!/bin/bash

cat system.zip0* > combined.zip
unzip combined.zip
rm system.zip0*
rm combined.zip