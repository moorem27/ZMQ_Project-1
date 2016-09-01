#!/bin/bash

cd build
cmake ..
time make -j7
if [[ $? == 0 ]]; then
	xterm -hold -e router_dealer/./client &
    xterm -hold -e router_dealer/./broker &
	xterm -hold -e router_dealer/./worker &
fi

