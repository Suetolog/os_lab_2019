#! /bin/bash
for i in "$@"
do 
let  "sum = i + sum"
done
echo "number of elements: $#"
let  "sum = sum/$#"
echo "average is $sum"