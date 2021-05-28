#!/bin/bash

g++ -O2 -o A A.cpp

score=0
for input in in/*.txt
do
  s=$(./A < $input 2>&1 | grep -oP '(?<=score: )\d+')
  echo $s
  score=$((score+s))
done
echo $score
