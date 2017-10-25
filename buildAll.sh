#!/bin/bash

# Build all example drivers


cd blockDriver
make $1
cd ../characterDriver
make $1
cd ../networkDriver
make $1

