#! /bin/bash

make
./raytra 3balls.scn 3balls.exr
./raytra ball.scn ball.exr
./raytra grid.scn grid.exr
./raytra color_ball.scn color_ball.exr
./raytra complexgrid.scn complexgrid.exr
./raytra basictrig.scn basictrig.exr
./raytra spheregridrotated.scn spheregridrotated.exr
# # ./raytra spiral.scn spiral.exr

open *.exr
