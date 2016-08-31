#!/bin/bash

cd build
cmake ..
time make -j7
if [[ $? == 0 ]]; then
    xterm -hold -e ./subscriber &
	xterm -hold -e ./publisher &
fi

