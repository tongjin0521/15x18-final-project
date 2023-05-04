#!/bin/bash

bash benchmark-ompOnly.sh ghc-5000-omp-auto.csv
sed -i 's/#define method auto/#define method static/g' src/gwo-OpenMP.cpp
make

bash benchmark-ompOnly.sh ghc-5000-omp-static.csv
sed -i 's/#define method static/#define method dynamic/g' src/gwo-OpenMP.cpp
make

bash benchmark-ompOnly.sh ghc-5000-omp-dynamic.csv
sed -i 's/#define method dynamic/#define method auto/g' src/gwo-OpenMP.cpp
make
