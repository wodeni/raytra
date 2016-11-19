#! /bin/bash
ssh clic "cd cs4160/raytra; make clean; make hardcoded; valgrind --leak-check=yes ./raytra $1 $2 $3 $4;"
scp clic:cs4160/raytra/$2 .
