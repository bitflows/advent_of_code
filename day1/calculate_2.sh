#!/bin/bash

value=0
for line in $(cat input)
do
  let value+=$line
  echo "line : $line : $value"
done
knut@tinkerboard:~/advent_of_code/day1$ cat calculate_2.sh
#!/bin/bash

value=0
declare -A histogram

while `true`
do
  echo "iter"
  for line in $(cat input)
  do
    let value+=$line
    #echo "line : $line : $value"
    if [[ ${histogram[$value]} -ge 1 ]]
    then
      echo "Value $value found ${histogram[$value]} times!"
      exit 0
    fi
    let histogram[$value]++
    #echo "Added hostogram[$value]==${histogram[$value]}"
  done
done
