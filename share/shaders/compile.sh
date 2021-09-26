#!/bin/bash

FILES="*.frag"
for f in $FILES
do
    echo $f
    glslangValidator -V $f -o $f.spv
done

FILES="*.vert"
for f in $FILES
do
    glslangValidator -V $f -o $f.spv
done
