#!/bin/bash

# loop for 1 to 4
for i in {1..4}
do 
    #copy the scene.txt file from the $i/ folder to the current folder
    cp $i/scene.txt .
    g++ main.cpp -o demo 
    ./demo
    
    echo "test case $i"
    #compare the stage1.txt stage2.txt and stage3.txt files with $i/stage1.txt $i/stage2.txt and $i/stage3.txt files
    
    
    diff -b stage1.txt $i/stage1.txt
    #if the files are same then print "stage1.txt passed" else print "stage1.txt failed"

    if [ $? -eq 0 ]
    then
        echo "stage1.txt passed"
    else
        echo "stage1.txt failed"
    fi
    diff -b stage2.txt $i/stage2.txt
    if [ $? -eq 0 ]
    then
        echo "stage2.txt passed"
    else
        echo "stage2.txt failed"
    fi
    diff -b stage3.txt $i/stage3.txt
    if [ $? -eq 0 ]
    then
        echo "stage3.txt passed"
    else
        echo "stage3.txt failed"
    fi


    rm scene.txt stage1.txt stage2.txt stage3.txt demo
done

   