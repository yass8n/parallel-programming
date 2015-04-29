#!/bin/bash 
COUNTER=200
while [  $COUNTER < 300 ]; do
qdel $COUNTER.acano01
let COUNTER=COUNTER+1 
done
