#!/bin/bash

# Build all example drivers

uname -r >> testOutput.log
cat /proc/cpuinfo >> testOutput.log
git log -1 >> testOutput.log
cd blockDriver
make $1 >> ../testOutput.log
echo "Entering character driver..."
cd ../characterDriver
make $1 >> ../testOutput.log
cp testData.csv ../testData.csv
echo "Entering network driver..."
cd ../networkDriver
make $1 >> ../testOutput.log
echo
echo "*************************************"
echo
echo "  Finished! Please copy and return:"
echo "       -testOutput.log"
echo "       -testData.csv"
echo
echo "  Or run again to collect more data!"
echo
echo "*************************************"
echo
