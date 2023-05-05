#!/bin/bash

sed -i 's/int max_iter = max(5000/int max_iter = max(100/g' src/svm.cpp
make

bash benchmark-fixed.sh ghc-100.csv

sed -i 's/int max_iter = max(100/int max_iter = max(500/g' src/svm.cpp
make

bash benchmark-fixed.sh ghc-500.csv

sed -i 's/int max_iter = max(500/int max_iter = max(1000/g' src/svm.cpp
make

bash benchmark-fixed.sh ghc-1000.csv

sed -i 's/int max_iter = max(1000/int max_iter = max(2000/g' src/svm.cpp
make

bash benchmark-fixed.sh ghc-2000.csv

sed -i 's/int max_iter = max(2000/int max_iter = max(5000/g' src/svm.cpp
make
