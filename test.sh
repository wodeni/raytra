#! /bin/bash

make
./raytra ball.scn ball.exr
./raytra ball2.scn ball2.exr
./raytra grid.scn grid.exr
./raytra grid2.scn grid2.exr
./raytra color_ball.scn color_ball.exr
# ./raytra spiral.scn spiral.exr
./raytra 3balls.scn 3balls.exr
./raytra 3balls2.scn 3balls2.exr

./raytra complexgrid.scn complexgrid.exr
open *.exr