#!/bin/bash
score=0
for input in in/*.txt
do
  s=$($1 < $input 2>&1 | grep -oP '(?<=score: )\d+')
  echo $s
  score=$((score+s))
done
echo "score: $score"
