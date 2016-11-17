#! /bin/bash

SCENE="$1"
IMAGE="$2"

ssh clic "cd cs4160/raytra; /home/hl2918/cs4160/oracle/hw1.5 $1 $2 $3 $4;"
scp clic:cs4160/raytra/$2 .
