#!/bin/sh

gcc main.c commands/* -o shell.out -lreadline && \
mv *.out out && \
out/shell.out