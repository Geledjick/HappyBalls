#!/bin/bash
rm -rf build
cmake --fresh -G "Unix Makefiles" -B build
./make.sh