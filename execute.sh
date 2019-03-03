#!/bin/sh

gcc main.c commands/* -o shell.out && \
mv *.out out && \
out/shell.out