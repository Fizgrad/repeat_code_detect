#!/bin/bash

applications=("telegram" "googleplay" "youtube")
modes=("origin" "hash")

for application in "${applications[@]}"
do
    for mode in "${modes[@]}"
    do
        ./build/suffix_tree --code ./data/code/$application --out ./data/res/${application}_${mode}.txt --mode $mode > ./data/log/${application}_${mode}.txt
    done
done
