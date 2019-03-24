#!/bin/sh

gcc main.c commands/* -o shell.out -lreadline -lm && \
mv *.out out && \
out/shell.out