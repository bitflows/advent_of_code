#!/bin/bash

value=0
for line in $(cat input)
do
  let value+=$line
  echo "line : $line : $value"
done
