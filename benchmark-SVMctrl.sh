#!/bin/bash

for i in {1..5}
do
  sed -i 's/int max_iter = max(5000/int max_iter = max(100/g' src/svm.cpp
  make

  bash benchmark-fixed.sh ghc-100-${i}.csv

  sed -i 's/int max_iter = max(100/int max_iter = max(500/g' src/svm.cpp
  make

  bash benchmark-fixed.sh ghc-500-${i}.csv

  sed -i 's/int max_iter = max(500/int max_iter = max(1000/g' src/svm.cpp
  make

  bash benchmark-fixed.sh ghc-1000-${i}.csv

  sed -i 's/int max_iter = max(1000/int max_iter = max(2000/g' src/svm.cpp
  make

  bash benchmark-fixed.sh ghc-2000-${i}.csv

  sed -i 's/int max_iter = max(2000/int max_iter = max(5000/g' src/svm.cpp
  make
done
