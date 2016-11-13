#! /bin/bash

# make
# ./raytra scene-files/3balls.scn 3balls.exr
# ./raytra scene-files/ball.scn ball.exr
# ./raytra scene-files/grid.scn grid.exr
# ./raytra scene-files/color_ball.scn color_ball.exr
# ./raytra scene-files/complexgrid.scn complexgrid.exr
# ./raytra scene-files/basictrig.scn basictrig.exr
# ./raytra scene-files/spheregridrotated.scn spheregridrotated.exr
# # ./raytra spiral.scn spiral.exr

make clean
make hardcoded
for f in scene-files-with-planes/*;
do
    fwextension=$(basename $f)
    filename="${fwextension%.*}"
    echo ./raytra $fwextension $filename $1
    time ./raytra $f  $filename.exr $1
done
open *.exr
