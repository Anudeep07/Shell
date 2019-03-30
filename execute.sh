#!/bin/sh

if [ ! -d "obj" ]
then
    mkdir obj
fi

if [ ! -d "obj/commands" ]
then
    mkdir obj
fi

if [ ! -d "out" ]
then
    mkdir out
fi

make -s && \
out/shell.out