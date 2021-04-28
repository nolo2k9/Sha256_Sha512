#!/bin/bash

echo "Checking file inputs.txt."
expec="$(shasum -a 512 inputs.txt)"
myout="$(./SHA512 inputs.txt)  inputs.txt"
echo "Expected: " $expec
echo "Output:   " $myout
if [[ $myout == $expec ]]; then
    echo "Pass"
else
    echo "Fail"
fi


